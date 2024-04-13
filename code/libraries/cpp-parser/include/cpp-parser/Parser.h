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

    TokenList& GetAST();
};

} // namespace parser
