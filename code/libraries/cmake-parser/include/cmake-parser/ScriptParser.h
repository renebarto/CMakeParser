#pragma once

#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/Lexer.h"
#include "parser/ParserExecutor.h"

namespace cmake_parser {

class ScriptParser
    : public parser::IParserCallback<Terminal>
{
private:
    std::filesystem::path m_path;
    Lexer m_lexer;
    parser::Token<Terminal> m_currentToken;
    int m_errorCount;
    CMakeModel& m_model;
    ProjectPtr m_mainProject;
    ProjectPtr m_currentProject;
    TargetPtr m_currentTarget;

public:
    ScriptParser(CMakeModel& model, const std::filesystem::path& rootDirectory, std::istream& stream);
    bool Parse();

    const CMakeModel& GetModel() const { return m_model; }

    std::string ParseVersion(const TerminalSet& endTerminals);
    std::string ParseDescription();
    std::string ParseLanguages(const TerminalSet& endTerminals);
    std::string ParseURL();

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
    std::filesystem::path ExpectPath(const std::set<Terminal>& finalizers);
    std::string ExpectExpression(const std::set<Terminal>& finalizers);
    std::string ExpectExpressionPart();

    std::string Evaluate(const std::string& expression) const;
    bool HandleCMakeMinimumRequired();
    bool HandleProject();
    bool HandleMessage();
    bool IsValidMessageMode(const std::string& mode);
    bool HandleSet();
    bool HandleUnset();
    bool HandleAddSubdirectory();
    bool HandleAddExecutable();
    bool HandleAddLibrary();
    bool HandleTargetIncludeDirectories();
    bool HandleTargetCompileDefinitions();
    bool HandleTargetCompileOptions();
    bool HandleUnsupported();

    bool OnToken(const parser::Token<Terminal>&, bool& done) override;
    bool OnNoMoreToken(const parser::SourceLocation& location) override;
    void OnSkipToken(const parser::Token<Terminal>& token) override;
    void OnParseError(const parser::Token<Terminal>& token) override;

    std::string Serialize() const;

private:
    std::string ReadScopedArguments();
};

} // namespace cmake_parser
