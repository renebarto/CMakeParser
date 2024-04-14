#pragma once

#include <set>
#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/Lexer.h"

namespace cmake_parser {

class Parser
{
private:
    Lexer m_lexer;
    parser::Token m_currentToken;
    int m_errorCount;
    CMakeModel m_model;

public:
    Parser(const std::string& path, std::istream& stream);
    bool Parse();

    const CMakeModel& GetModel() const { return m_model; }

    void NextToken();
    Terminal CurrentTokenType();
    Terminal TokenType(const parser::Token& token);
    void SkipWhitespace();
    std::string Expect(Terminal type);
    parser::Token Expect(std::set<Terminal> oneOfTypes);
    std::string ExpectVariable();

    bool HandleKeyword();
    void HandleCMakeMinimumRequired();
    void HandleMessage();
    bool IsValidMessageMode(const std::string& mode);
    void HandleSet();
    void HandleIf();
    std::string GetExpression(const parser::Token& token);

    void OnNoMoreToken(const parser::SourceLocation& location);
    void OnParseError(const std::string& text, const parser::SourceLocation& startLocation, const parser::SourceLocation& endLocation);
};

} // namespace cmake_parser
