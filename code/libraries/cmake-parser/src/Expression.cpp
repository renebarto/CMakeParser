#include "cmake-parser/Expression.h"

#include "tracing/Tracing.h"

using namespace parser;

namespace cmake_parser {
namespace expression {

class UnexpectedToken
    : public std::exception
{
private:
    std::string m_message;
    parser::SourceLocation m_location;

public:
    UnexpectedToken(const parser::Token<Terminal>& token, const char* fileName, int line)
        : m_message{ "Unexpected token: " + token.Serialize() }
        , m_location{ fileName, line, 1 }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
    const parser::SourceLocation& Location() const { return m_location; }
};

static void PrintToken(const Token<Terminal>& token)
{
    TRACE_DEBUG("Token: {}", token);
}

static void PrintUngetToken(const Token<Terminal>& token)
{
    TRACE_DEBUG("Unget: {}", token);
}

static void PrintTokenSkip(const Token<Terminal>& token)
{
    TRACE_DEBUG("Skip: {}", token);
}

Expression::Expression(const CMakeModel& model, const std::string& expression)
    : m_model{ model }
    , m_stream{ expression }
    , m_lexer{ "", m_stream }
    , m_currentToken{}
    , m_errorCount{}
    , m_result{}
{
}

std::string Expression::Evaluate()
{
    if (Parse())
    {
        return m_result;
    }
    return {};
}

std::string Expression::EvaluateString(const std::string& text)
{
    std::string result;
    size_t index{};
    while (index < text.length())
    {
        char ch = text[index++];
        switch (ch)
        {
        case '$':
            if ((index < text.length()) && (text[index] == '{'))
            {
                index++;
                std::string variableName;
                while ((index < text.length()) && (text[index] != '}'))
                {
                    variableName += text[index++];
                }
                if ((index < text.length()) && (text[index] == '}'))
                {
                    result += m_model.GetVariable(variableName);
                    index++;
                }
                else
                {
                    // Invalid variable expression
                    result += "${" + variableName;
                }
            }
            break;
        default:
            result += ch;
            break;
        }
    }
    return result;
}

bool Expression::Parse()
{
    m_result = {};
    ParserExecutor<Terminal> parserExecutor(*this, { Terminal::Whitespace, Terminal::NewLine });

    return parserExecutor.Parse(m_lexer) && (m_errorCount == 0);
}

void Expression::NextToken()
{
    m_currentToken = m_lexer.GetToken();
    PrintToken(CurrentToken());
}

Terminal Expression::CurrentTokenType()
{
    return CurrentToken().Type().TypeCode();
}

const parser::Token<Terminal>& Expression::CurrentToken()
{
    return m_currentToken;
}

void Expression::UngetCurrentToken()
{
    m_lexer.UngetToken(CurrentToken());
    PrintUngetToken(CurrentToken());
}

void Expression::SkipWhitespace()
{
    while ((CurrentTokenType() == Terminal::Whitespace) || (CurrentTokenType() == Terminal::NewLine))
    {
        NextToken();
    }
}

std::string Expression::Expect(Terminal type)
{
    if (CurrentTokenType() != type)
    {
        OnParseError(CurrentToken());
        throw UnexpectedToken(CurrentToken(), __FILE__, __LINE__);
    }
    auto result = CurrentToken().Value();
    NextToken();
    return result;
}

std::string Expression::Expect_SkipWhitespace(Terminal type)
{
    SkipWhitespace();
    return Expect(type);
}

bool Expression::OnToken(const parser::Token<Terminal>& token, bool& done)
{
    done = false;
    PrintToken(token);
    switch (token.Type().TypeCode())
    {
    case Terminal::String:
        m_result += EvaluateString(token.Value());
        break;
    case Terminal::Dollar:
        {
            NextToken();
            Expect(Terminal::CurlyBracketOpen);
            auto variableName = Expect(Terminal::Name);
            Expect(Terminal::CurlyBracketClose);
            UngetCurrentToken();
            m_result += m_model.GetVariable(variableName);
        }
        break;
    case Terminal::DigitSequence:
        {
            m_result = token.Value();
            NextToken();
            m_result += Expect(Terminal::Dot);
            m_result += Expect(Terminal::DigitSequence);
            if (CurrentToken().IsNull())
            {
                UngetCurrentToken();
                return true;
            }
            m_result += Expect(Terminal::Dot);
            m_result += Expect(Terminal::DigitSequence);
            if (CurrentToken().IsNull())
            {
                UngetCurrentToken();
                return true;
            }
            m_result += Expect(Terminal::Dot);
            m_result += Expect(Terminal::DigitSequence);
        }
        break;
    default:
        m_result += token.Value();
        break;
    }

    return true;
}

bool Expression::OnNoMoreToken(const parser::SourceLocation& /*location*/)
{
    TRACE_INFO("No more tokens");
    return true;
}

void Expression::OnSkipToken(const parser::Token<Terminal>& token)
{
    PrintTokenSkip(token);
}

void Expression::OnParseError(const parser::Token<Terminal>& token)
{
    m_errorCount++;
    TRACE_ERROR("Parse error: unexpected token {}", token);
}

} // namespace expression
} // namespace cmake_parser