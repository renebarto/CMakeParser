#include "cmake-parser/Parser.h"

#include "utility/CommandLine.h"
#include "utility/Console.h"
#include "utility/StringFunctions.h"
#include "cmake-parser/ParserExceptions.h"

using namespace parser;

namespace cmake_parser {

static utility::Console s_console;

static const std::string MessageModeFatalError{ "FATAL_ERROR" };
static const std::string MessageModeSendError{ "SEND_ERROR" };
static const std::string MessageModeWarning{ "WARNING" };
static const std::string MessageModeAuthorWarning{ "AUTHOR_WARNING" };
static const std::string MessageModeDeprecation{ "DEPRECATION" };
static const std::string MessageModeNotice{ "NOTICE" };
static const std::string MessageModeStatus{ "STATUS" };
static const std::string MessageModeVerbose{ "VERBOSE" };
static const std::string MessageModeDebug{ "DEBUG" };
static const std::string MessageModeTrace{ "TRACE" };
static const std::string EnvironmentPrefix{ "ENV" };
static const std::string KeywordCache{ "CACHE" };
static const std::string KeywordParentScope{ "PARENT_SCOPE" };
static const std::string KeywordForce{ "FORCE" };

static void PrintToken(const Token& token)
{
    s_console << fgcolor(utility::ConsoleColor::Magenta) << token << fgcolor(utility::ConsoleColor::Default) << std::endl;
}

static std::string FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

static std::string GetCMakeVersion()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("cmake --version", "", stdoutText, stderrText) == 0)
    {
        auto line = stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
        std::string prefix = "cmake version ";
        return line.substr(prefix.length());
    }
    return {};
}

static std::string FindNinja()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

Parser::Parser(const std::string& path, std::istream& stream)
	: m_lexer(path, stream)
    , m_errorCount{}
    , m_model{}
{
    m_model.SetupSourceRoot(path);
    m_model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    m_model.SetupNinjaPath(FindNinja());
}

bool Parser::Parse()
{
    NextToken();
    while (!m_lexer.IsAtEnd())
    {
        SkipWhitespace();
        if (m_currentToken.IsInvalid() || (m_currentToken.IsNull() && !m_lexer.IsAtEnd()))
        {
            return false;
        }
        if (m_currentToken.IsNull())
        {
            OnNoMoreToken(m_lexer.GetCurrentLocation());
            break;
        }
        else
        {
            if (!HandleKeyword())
                return false;
        }
    }
    return m_lexer.IsAtEnd() && (m_errorCount == 0);
}

void Parser::NextToken()
{
    m_currentToken = m_lexer.GetToken();
    PrintToken(m_currentToken);
}

Terminal Parser::CurrentTokenType()
{
    return TokenType(m_currentToken);
}

Terminal Parser::TokenType(const Token& token)
{
    return static_cast<Terminal>(token.Type().m_type);
}

void Parser::SkipWhitespace()
{
    while ((CurrentTokenType() == Terminal::Whitespace) || (CurrentTokenType() == Terminal::NewLine))
    {
        NextToken();
    }
}


std::string Parser::Expect(Terminal type)
{
    SkipWhitespace();
    if (CurrentTokenType() != type)
    {
        OnParseError(m_currentToken.Value(), m_currentToken.BeginLocation(), m_currentToken.EndLocation());
        throw UnexpectedToken(m_currentToken);
    }
    auto result = m_currentToken.Value();
    NextToken();
    return result;
}

Token Parser::Expect(std::set<Terminal> oneOfTypes)
{
    SkipWhitespace();
    if (oneOfTypes.find(CurrentTokenType()) == oneOfTypes.end())
    {
        OnParseError(m_currentToken.Value(), m_currentToken.BeginLocation(), m_currentToken.EndLocation());
        throw UnexpectedToken(m_currentToken);
    }
    auto result = m_currentToken;
    NextToken();
    return result;
}

std::string Parser::ExpectVariable()
{
    std::string result;

    Expect(Terminal::CurlyBracketOpen);
    result = Expect(Terminal::Identifier);
    Expect(Terminal::CurlyBracketClose);

    return result;
}

bool Parser::HandleKeyword()
{
    bool result{ true };
    try
    {
        switch (CurrentTokenType())
        {
        case CMakeMinimumRequiredKeyword:
            HandleCMakeMinimumRequired();
            break;
        case MessageKeyword:
            HandleMessage();
            break;
        case SetKeyword:
            HandleSet();
            break;
        case IfKeyword:
            HandleIf();
            break;
        default:
            throw UnexpectedToken(m_currentToken);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Parsing failed: " << e.what() << std::endl;
        result = false;
    }

    return result;
}

void Parser::HandleCMakeMinimumRequired()
{
    Expect(Terminal::CMakeMinimumRequiredKeyword);
    Expect(Terminal::ParenthesisOpen);
    Expect(Terminal::VersionKeyword);
    auto version = Expect(Terminal::VersionNumber);
    Expect(Terminal::ParenthesisClose);
    m_model.SetVariable("CMAKE_MINIMUM_REQUIRED_VERSION", version);
}

void Parser::HandleMessage()
{
    Expect(Terminal::MessageKeyword);
    Expect(Terminal::ParenthesisOpen);
    auto token = Expect({ Terminal::Identifier, Terminal::String });
    std::string messageMode;
    std::string message;
    if (TokenType(token) == Terminal::Identifier)
    {
        if (!IsValidMessageMode(token.Value()))
            throw InvalidMessageMode(token);
        messageMode = token.Value();
    }
    else
    {
        messageMode = MessageModeNotice;
    }
    message = Expect(Terminal::String);
    m_model.AddMessage(messageMode, message);
    Expect(Terminal::ParenthesisClose);
}

bool Parser::IsValidMessageMode(const std::string& mode)
{
    return
        utility::IsEqualIgnoreCase(MessageModeFatalError, mode) ||
        utility::IsEqualIgnoreCase(MessageModeSendError, mode) ||
        utility::IsEqualIgnoreCase(MessageModeWarning, mode) ||
        utility::IsEqualIgnoreCase(MessageModeAuthorWarning, mode) ||
        utility::IsEqualIgnoreCase(MessageModeDeprecation, mode) ||
        utility::IsEqualIgnoreCase(MessageModeNotice, mode) ||
        utility::IsEqualIgnoreCase(MessageModeStatus, mode) ||
        utility::IsEqualIgnoreCase(MessageModeVerbose, mode) ||
        utility::IsEqualIgnoreCase(MessageModeDebug, mode) ||
        utility::IsEqualIgnoreCase(MessageModeTrace, mode);
}

void Parser::HandleSet()
{
    std::string variableName;
    std::string value;
    Expect(Terminal::SetKeyword);
    Expect(Terminal::ParenthesisOpen);
    auto identifier = Expect(Terminal::Identifier);
    if (utility::IsEqualIgnoreCase(EnvironmentPrefix, identifier))
    {
        variableName = ExpectVariable();
        auto token = Expect({ Terminal::String, Terminal::Dollar, Terminal::ParenthesisClose });
        if (TokenType(token) != Terminal::ParenthesisClose)
        {
            value = GetExpression(token);
            Expect(Terminal::ParenthesisClose);
        }
    }
    else
    {
        variableName = identifier;
        auto token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
        if (TokenType(token) != Terminal::ParenthesisClose)
        {
            value = GetExpression(token);
            token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
        }
        while (TokenType(token) != Terminal::ParenthesisClose)
        {
            if (utility::IsEqualIgnoreCase(KeywordCache, token.Value()))
            {
                // todo save attributes
                Expect({ Terminal::Identifier, Terminal::StringKeyword });
                Expect(Terminal::String);
                token = Expect({ Terminal::Identifier, Terminal::ParenthesisClose });
                if (TokenType(token) != Terminal::ParenthesisClose)
                {
                    if (utility::IsEqualIgnoreCase(KeywordForce, token.Value()))
                    {
                        Expect(Terminal::ParenthesisClose);
                    }
                    else
                        throw UnexpectedToken(token);
                }
            }
            else if (utility::IsEqualIgnoreCase(KeywordParentScope, token.Value()))
            {
                // todo save attributes
            }
            else
            {
                value += " " + GetExpression(token);
                token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
            }
        }
    }
    m_model.SetVariable(variableName, value);
}

void Parser::HandleIf()
{
}

std::string Parser::GetExpression(const Token& token)
{
    std::string result;
    switch (TokenType(token))
    {
        case Terminal::String:
            result = token.Value();
            break;
        case Terminal::Identifier:
            result = token.Value();
            break;
        case Terminal::Dollar:
            result = "${" + ExpectVariable() + "}";
            break;
        default:
            throw UnexpectedToken(token);
    }
    return result;
}

void Parser::OnNoMoreToken(const SourceLocation& /*location*/)
{
    std::cout << "No more tokens" << std::endl;
}

void Parser::OnParseError(const std::string& text, const SourceLocation& startLocation, const SourceLocation& endLocation)
{
    m_errorCount++;
    std::cout << "Parse error: unexpected token " << text << " at location " << startLocation << "-" << endLocation << std::endl;
}

} // namespace cmake_parser
