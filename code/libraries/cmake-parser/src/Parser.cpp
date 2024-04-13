#include "cmake-parser/Parser.h"

using namespace parser;

namespace cmake_parser {

Parser::Parser(const std::string& path, std::istream& stream)
	: m_lexer(path, stream)
    , m_errorCount{}
{
}

bool Parser::Parse()
{
    while (!m_lexer.IsAtEnd())
    {
        auto token = m_lexer.GetToken();
        if (token.IsNull())
        {
            OnNoMoreToken(m_lexer.GetCurrentLocation());
        }
        if (token.IsInvalid() || (token.IsNull() && !m_lexer.IsAtEnd()))
        {
            return false;
            break;
        }
        if (!token.IsNull())
        {
        }
    }
    return m_lexer.IsAtEnd() && (m_errorCount == 0);
}

Terminal Parser::Type(parser::Token& token)
{
    return static_cast<Terminal>(token.Type().m_type);
}

bool Parser::Expect(Terminal type, Token& token)
{
    do
    {
        token = m_lexer.GetToken();
    } while (Type(token) == Terminal::Whitespace);
    if (Type(token) != type)
    {
        OnParseError(token.Value(), token.BeginLocation(), token.EndLocation());
        return false;
    }
    return true;
}

bool Parser::Expect(std::set<Terminal> oneOfTypes, Token& token)
{
    do
    {
        token = m_lexer.GetToken();
    } while (Type(token) == Terminal::Whitespace);
    if (oneOfTypes.find(Type(token)) == oneOfTypes.end())
    {
        OnParseError(token.Value(), token.BeginLocation(), token.EndLocation());
        return false;
    }
    return true;
}

void Parser::OnNoMoreToken(const SourceLocation& location)
{
    std::cout << "No more tokens" << std::endl;
}

void Parser::OnParseError(const std::string& text, const SourceLocation& startLocation, const SourceLocation& endLocation)
{
    m_errorCount++;
    std::cout << "Parse error: unexpected token " << text << " at location " << startLocation << "-" << endLocation << std::endl;
}

} // namespace cmake_parser
