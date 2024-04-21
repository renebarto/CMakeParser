#include "cmake-parser/Parser.h"

#include "serialization/EnumSerialization.h"
#include "utility/CommandLine.h"
#include "utility/Console.h"
#include "utility/StringFunctions.h"
#include "cmake-parser/ParserExceptions.h"

using namespace utility;
using namespace parser;
using namespace std::placeholders;

namespace cmake_parser {

static Console s_console;

using Data = Token<Terminal>;

static const std::string VersionKeyword{ "version" };
static const std::string DescriptionKeyword{ "description" };
static const std::string LanguagesKeyword{ "languages" };

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

enum class Keyword {
    AddSubDirectory,
    Cache,
    CMakeMinimumRequired,
    Else,
    EndForEach,
    EndIf,
    FindPackage,
    Force,
    ForEach,
    If,
    Include,
    List,
    Message,
    Option,
    ParentScope,
    Project,
    Set,
    String,
    Unset,
    Version,
};

} // cmake_parser

namespace serialization {

template<>
const BidirectionalMap<cmake_parser::Keyword, std::string> EnumSerializationMap<cmake_parser::Keyword>::ConversionMap =
{
    { cmake_parser::Keyword::AddSubDirectory, "add_subdirectory" },
    { cmake_parser::Keyword::Cache, "cache" },
    { cmake_parser::Keyword::CMakeMinimumRequired, "cmake_minimum_required" },
    { cmake_parser::Keyword::Else, "else" },
    { cmake_parser::Keyword::EndForEach, "endforeach" },
    { cmake_parser::Keyword::EndIf, "endif" },
    { cmake_parser::Keyword::FindPackage, "find_package" },
    { cmake_parser::Keyword::Force, "force" },
    { cmake_parser::Keyword::ForEach, "foreach" },
    { cmake_parser::Keyword::If, "if" },
    { cmake_parser::Keyword::Include, "include" },
    { cmake_parser::Keyword::List, "list" },
    { cmake_parser::Keyword::Message, "message" },
    { cmake_parser::Keyword::Option, "option" },
    { cmake_parser::Keyword::ParentScope, "parent_scope" },
    { cmake_parser::Keyword::Project, "project" },
    { cmake_parser::Keyword::Set, "set" },
    { cmake_parser::Keyword::String, "string" },
    { cmake_parser::Keyword::Unset, "unset" },
    { cmake_parser::Keyword::Version, cmake_parser::VersionKeyword },
};

} // namespace serialization

namespace cmake_parser {

inline std::ostream& operator << (std::ostream& stream, Keyword keyword)
{
    return stream << serialization::Serialize(keyword);
}

inline std::ostream& operator << (std::ostream& stream, State state)
{
    return stream << serialization::Serialize(state);
}

static bool LookupKeyword(const Token<Terminal>& token, Keyword& keyword)
{
    for (auto const& item : serialization::EnumSerializationMap<Keyword>::GetValues())
    {
        if (utility::IsEqualIgnoreCase(item, token.Value()))
        {
            serialization::Deserialize(item, keyword);
            return true;
        }
    }
    return false;
}

static void PrintColor(ConsoleColor color, const std::string& text)
{
    s_console << fgcolor(color) << text << fgcolor(ConsoleColor::Default) << std::endl;
}

static void PrintToken(const Token<Terminal>& token)
{
    s_console << fgcolor(ConsoleColor::Magenta) << token << fgcolor(ConsoleColor::Default) << std::endl;
}

static void PrintUngetToken(const Token<Terminal>& token)
{
    s_console << fgcolor(ConsoleColor::Magenta) << "Unget " << token << fgcolor(ConsoleColor::Default) << std::endl;
}

static void PrintTokenSkip(const Token<Terminal>& token)
{
    s_console << fgcolor(ConsoleColor::Magenta) << token << " SKIP" << fgcolor(ConsoleColor::Default) << std::endl;
}

static std::string FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

static std::string GetCMakeVersion()
{
    std::string stdoutText;
    std::string stderrText;
    if (SystemRedirect("cmake --version", "", stdoutText, stderrText) == 0)
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
    if (SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

Parser::Parser(const std::string& path, std::istream& stream)
	: m_lexer(path, stream)
    , m_errorCount{}
    , m_model{}
    , m_mainProject{}
    , m_currentProject{}
{
    m_model.SetupSourceRoot(path);
    m_model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    m_model.SetupNinjaPath(FindNinja());
}

bool Parser::Parse()
{
    ParserExecutor<Terminal> parserExecutor(*this, { Terminal::Whitespace, Terminal::NewLine });

    return parserExecutor.Parse(m_lexer) && (m_errorCount == 0);
}

std::string Parser::ParseVersion(const TerminalSet& endTerminals)
{
    std::string version;

    try
    {
        NextToken();
        SkipWhitespace();
        version = Expect(Terminal::DigitSequence);
        version += Expect(Terminal::Dot);
        version += Expect(Terminal::DigitSequence);
        if (CurrentTokenInSet(endTerminals))
        {
            UngetCurrentToken();
            return version;
        }
        version += Expect(Terminal::Dot);
        version += Expect(Terminal::DigitSequence);
        if (CurrentTokenInSet(endTerminals))
        {
            UngetCurrentToken();
            return version;
        }
        version += Expect(Terminal::Dot);
        version += Expect(Terminal::DigitSequence);
    }
    catch (std::exception&)
    { 
        return {};
    }

    return version;
}

std::string Parser::ParseDescription()
{
    std::string description;

    try
    {
        NextToken();
        SkipWhitespace();
        description = Expect(Terminal::String);
    }
    catch (std::exception&)
    {
        return {};
    }

    return description;
}

std::string Parser::ParseLanguages(const TerminalSet& endTerminals)
{
    std::string languages;

    try
    {
        NextToken();
        SkipWhitespace();
        languages = Expect(Terminal::Identifier);
        SkipWhitespace();
        while (!CurrentTokenInSet(endTerminals))
        {
            languages += " " + Expect(Terminal::Identifier);
            SkipWhitespace();
        }
        UngetCurrentToken();
    }
    catch (std::exception&)
    {
        return {};
    }

    return languages;
}

void Parser::NextToken()
{
    m_currentToken = m_lexer.GetToken();
    PrintToken(CurrentToken());
}

Terminal Parser::CurrentTokenType()
{
    return CurrentToken().Type().m_type;
}

const parser::Token<Terminal>& Parser::CurrentToken()
{
    return m_currentToken;
}

bool Parser::CurrentTokenInSet(const TerminalSet& terminals)
{
    return terminals.find(CurrentTokenType()) != terminals.end();
}

void Parser::UngetCurrentToken()
{
    m_lexer.UngetToken(CurrentToken());
    PrintUngetToken(CurrentToken());
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
    if (CurrentTokenType() != type)
    {
        OnParseError(CurrentToken());
        throw UnexpectedToken(CurrentToken());
    }
    auto result = CurrentToken().Value();
    NextToken();
    return result;
}

std::string Parser::Expect_SkipWhitespace(Terminal type)
{
    SkipWhitespace();
    return Expect(type);
}

Token<Terminal> Parser::Expect(std::set<Terminal> oneOfTypes)
{
    if (oneOfTypes.find(CurrentTokenType()) == oneOfTypes.end())
    {
        OnParseError(CurrentToken());
        throw UnexpectedToken(CurrentToken());
    }
    auto result = CurrentToken();
    NextToken();
    return result;
}

Token<Terminal> Parser::Expect_SkipWhitespace(std::set<Terminal> oneOfTypes)
{
    SkipWhitespace();
    return Expect(oneOfTypes);
}

//void Parser::HandleIf()
//{
//}
//
//std::string Parser::GetExpression(const Token<Terminal>& token)
//{
//    std::string result;
//    switch (token.Type().m_type)
//    {
//        case Terminal::String:
//            result = token.Value();
//            break;
//        case Terminal::Identifier:
//            result = token.Value();
//            break;
//        case Terminal::Dollar:
//            result = "${" + ExpectVariable() + "}";
//            break;
//        default:
//            throw UnexpectedToken(token);
//    }
//    return result;
//}

bool Parser::HandleCMakeMinimumRequired()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    std::string str = Expect_SkipWhitespace(Terminal::Identifier);
    if (!utility::IsEqualIgnoreCase(VersionKeyword, str))
        return false;
    auto result = ParseVersion({ Terminal::Whitespace, Terminal::NewLine, Terminal::ParenthesisClose });
    if (result.empty())
        return false;
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    m_model.SetVariable("CMAKE_MINIMUM_REQUIRED_VERSION", result);
    return true;
}

bool Parser::HandleProject()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto result = Expect_SkipWhitespace(Terminal::Name);
    m_currentProject = std::make_shared<Project>(result);
    SkipWhitespace();
    while (CurrentTokenType() == Terminal::Identifier)
    {
        result = Expect(Terminal::Identifier);
        if (utility::IsEqualIgnoreCase(VersionKeyword, result))
        {
            result = ParseVersion({ Terminal::Whitespace, Terminal::NewLine, Terminal::ParenthesisClose });
            if (!result.empty())
            {
                m_currentProject->SetVersion(result);
            }
        }
        else if (utility::IsEqualIgnoreCase(DescriptionKeyword, result))
        {
            result = ParseDescription();
            if (!result.empty())
            {
                m_currentProject->SetDescription(result);
            }
        }
        else if (utility::IsEqualIgnoreCase(LanguagesKeyword, result))
        {
            result = ParseLanguages({ Terminal::ParenthesisClose });
            if (!result.empty())
            {
                m_currentProject->SetLanguages(result);
            }
        }
        else
        {
            return false;
        }
        SkipWhitespace();
    }
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool Parser::HandleMessage()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto token = Expect_SkipWhitespace({ Terminal::Identifier, Terminal::String });
    std::string messageMode;
    std::string message;
    if (token.Type() == Terminal::Identifier)
    {
        if (!IsValidMessageMode(token.Value()))
        {
            return false;
        }
        messageMode = token.Value();
        message = Expect_SkipWhitespace(Terminal::String);
    }
    else
    {
        messageMode = MessageModeNotice;
        message = token.Value();
    }
    m_model.AddMessage(messageMode, message);
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool Parser::IsValidMessageMode(const std::string& mode)
{
    return
        IsEqualIgnoreCase(MessageModeFatalError, mode) ||
        IsEqualIgnoreCase(MessageModeSendError, mode) ||
        IsEqualIgnoreCase(MessageModeWarning, mode) ||
        IsEqualIgnoreCase(MessageModeAuthorWarning, mode) ||
        IsEqualIgnoreCase(MessageModeDeprecation, mode) ||
        IsEqualIgnoreCase(MessageModeNotice, mode) ||
        IsEqualIgnoreCase(MessageModeStatus, mode) ||
        IsEqualIgnoreCase(MessageModeVerbose, mode) ||
        IsEqualIgnoreCase(MessageModeDebug, mode) ||
        IsEqualIgnoreCase(MessageModeTrace, mode);
}

bool Parser::HandleSet()
{
    std::string variableName;
    std::string value;
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto result = Expect_SkipWhitespace(Terminal::Identifier);
    if (IsEqualIgnoreCase(EnvironmentPrefix, result))
    {
        variableName = ExpectVariable();
        auto token = Expect({ Terminal::String, Terminal::Dollar, Terminal::ParenthesisClose });
        if (token.Type() != Terminal::ParenthesisClose)
        {
            //value = GetExpression(token);
        }
    }
    else
    {
        variableName = result;
        auto token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
        if (token.Type() != Terminal::ParenthesisClose)
        {
            //value = GetExpression(token);
            token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
        }
        while (token.Type() != Terminal::ParenthesisClose)
        {
            if (IsEqualIgnoreCase(KeywordCache, token.Value()))
            {
                // todo save attributes
                Expect({ Terminal::Identifier, Terminal::String });
                Expect(Terminal::String);
                token = Expect({ Terminal::Identifier, Terminal::ParenthesisClose });
                if (token.Type() != Terminal::ParenthesisClose)
                {
                    if (IsEqualIgnoreCase(KeywordForce, token.Value()))
                    {
                        Expect(Terminal::ParenthesisClose);
                    }
                    else
                        throw UnexpectedToken(token);
                }
            }
            else if (IsEqualIgnoreCase(KeywordParentScope, token.Value()))
            {
                // todo save attributes
            }
            else
            {
                //value += " " + GetExpression(token);
                token = Expect({ Terminal::String, Terminal::Identifier, Terminal::Dollar, Terminal::ParenthesisClose });
            }
        }
    }
    m_model.SetVariable(variableName, value);
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
}

bool Parser::HandleUnset()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto result = Expect_SkipWhitespace(Terminal::Identifier);
    bool cache{};
    bool parentScope{};
    while (CurrentTokenType() == Terminal::Identifier)
    {
        Keyword keyword;
        LookupKeyword(CurrentToken(), keyword);

    }
    auto token = Expect_SkipWhitespace({ Terminal::Identifier, Terminal::ParenthesisClose });
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

std::string Parser::ExpectVariable()
{
    std::string result;

    Expect(Terminal::CurlyBracketOpen);
    result = Expect(Terminal::Identifier);
    Expect(Terminal::CurlyBracketClose);

    return result;
}

bool Parser::OnToken(const parser::Token<Terminal>& token, bool& done)
{
    PrintToken(token);
    NextToken();
    Keyword keyword;
    if (!LookupKeyword(token, keyword))
    {
        return false;
    }
    bool result{};
    switch (keyword)
    {
    case Keyword::CMakeMinimumRequired:
        result = HandleCMakeMinimumRequired();
        break;
    case Keyword::Project:
        result = HandleProject();
        break;
    case Keyword::Message:
        result = HandleMessage();
        break;
    case Keyword::Set:
        result = HandleSet();
        break;
    case Keyword::Unset:
        result = HandleUnset();
        break;
    default:
        break;
    }

    done = false;
    return result;
}

bool Parser::OnNoMoreToken(const parser::SourceLocation& /*location*/)
{
    PrintColor(ConsoleColor::Green, "No more tokens");
    return true;
}

void Parser::OnSkipToken(const parser::Token<Terminal>& token)
{
    PrintTokenSkip(token);
}

void Parser::OnParseError(const parser::Token<Terminal>& token)
{
    m_errorCount++;
    std::ostringstream stream;
    stream << "Parse error: unexpected token " << token;
    PrintColor(ConsoleColor::Red, stream.str());
}

} // namespace cmake_parser
