#pragma once

#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/Lexer.h"
#include "parser/ParserExecutor.h"

namespace cmake_parser {

enum class State;

class Parser
    : public parser::IParserCallback<Terminal>
{
private:
    Lexer m_lexer;
    parser::Token<Terminal> m_currentToken;
    int m_errorCount;
    CMakeModel m_model;
    ProjectPtr m_mainProject;
    ProjectPtr m_currentProject;

public:
    Parser(const std::filesystem::path& rootDurectory, const std::string& fileName, std::istream& stream);
    bool Parse();

    const CMakeModel& GetModel() const { return m_model; }

    std::string ParseVersion(const TerminalSet& endTerminals);
    std::string ParseDescription();
    std::string ParseLanguages(const TerminalSet& endTerminals);

    void NextToken();
    const parser::Token<Terminal>& CurrentToken();
    Terminal CurrentTokenType();
    bool CurrentTokenInSet(const TerminalSet& terminals);
    void UngetCurrentToken();
    void SkipWhitespace();
    std::string Expect(Terminal type);
    std::string Expect_SkipWhitespace(Terminal type);
    parser::Token<Terminal> Expect(std::set<Terminal> oneOfTypes);
    parser::Token<Terminal> Expect_SkipWhitespace(std::set<Terminal> oneOfTypes);
    std::string ExpectVariable();
    std::string ExpectExpression(const std::set<Terminal>& finalizers);
    std::string ExpectExpressionPart();

    //bool HandleKeyword();
    bool HandleCMakeMinimumRequired();
    bool HandleProject();
    bool HandleMessage();
    bool IsValidMessageMode(const std::string& mode);
    bool HandleSet();
    bool HandleUnset();
    //void HandleIf();
    //std::string GetExpression(const parser::Token<Terminal>& token);

    bool OnToken(const parser::Token<Terminal>&, bool& done) override;
    bool OnNoMoreToken(const parser::SourceLocation& location) override;
    void OnSkipToken(const parser::Token<Terminal>& token) override;
    void OnParseError(const parser::Token<Terminal>& token) override;
};

} // namespace cmake_parser
