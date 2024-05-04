#pragma once

#include <string>
#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/ExpressionLexer.h"
#include "parser/ParserExecutor.h"

namespace cmake_parser {
namespace expression {

class Expression
    : public parser::IParserCallback<Terminal>
{
private:
    const CMakeModel& m_model;
    std::istringstream m_stream;
    Lexer m_lexer;
    parser::Token<Terminal> m_currentToken;
    int m_errorCount;
    std::string m_result;

public:
    Expression(const CMakeModel& model, const std::string& expression);
    bool Parse();

    void NextToken();
    const parser::Token<Terminal>& CurrentToken();
    Terminal CurrentTokenType();
    void UngetCurrentToken();
    void SkipWhitespace();
    std::string Expect(Terminal type);
    std::string Expect_SkipWhitespace(Terminal type);
    std::string Evaluate();
    std::string EvaluateString(const std::string& text);
    static std::string EvaluateString(const CMakeModel& model, const std::string& text);

    bool OnToken(const parser::Token<Terminal>&, bool& done) override;
    bool OnNoMoreToken(const parser::SourceLocation& location) override;
    void OnSkipToken(const parser::Token<Terminal>& token) override;
    void OnParseError(const parser::Token<Terminal>& token) override;
};

} // namespace expression
} // namespace cmake_parser
