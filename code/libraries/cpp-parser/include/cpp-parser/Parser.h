#pragma once

#include "cpp-parser/Lexer.h"

namespace parser {

class Parser
{
private:
    Lexer m_lexer;

public:
    Parser(const std::string& path, std::istream& stream);
    bool Parse();

    TokenList<TokenTypes>& GetAST();
};

Parser::Parser(const std::string& path, std::istream& stream)
    : m_lexer(path, stream)
{
}

bool Parser::Parse()
{
    return m_lexer.Parse();
}

TokenList<TokenTypes>& Parser::GetAST()
{
    return m_lexer.GetTokens();
}

} // namespace parser
