#pragma once

#include <set>
#include "cmake-parser/Lexer.h"

namespace cmake_parser {

class Parser
{
private:
    Lexer m_lexer;
    int m_errorCount;

public:
    Parser(const std::string& path, std::istream& stream);
    bool Parse();

    Terminal Type(parser::Token& token);
    bool Expect(Terminal type, parser::Token& token);
    bool Expect(std::set<Terminal> oneOfTypes, parser::Token& token);

    void OnNoMoreToken(const parser::SourceLocation& location);
    void OnParseError(const std::string& text, const parser::SourceLocation& startLocation, const parser::SourceLocation& endLocation);
};

} // namespace cmake_parser
