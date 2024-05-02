#include "cmake-parser/ExpressionLexer.h"

#include "parser/TokenizerRule.h"

using namespace parser;

namespace cmake_parser {
namespace expression {

#define TOKEN_DEF(x) { Terminal::x, #x }
static TokenDefinitions<Terminal> tokenDefinitions {
    TOKEN_DEF(None),
    TOKEN_DEF(Whitespace),
    TOKEN_DEF(NewLine),
    TOKEN_DEF(Dollar),
    TOKEN_DEF(ForwardSlash),
    TOKEN_DEF(Equals),
    TOKEN_DEF(Dot),
    TOKEN_DEF(String),
    TOKEN_DEF(CurlyBracketOpen),
    TOKEN_DEF(CurlyBracketClose),
    TOKEN_DEF(Name),
    TOKEN_DEF(Comment),
    TOKEN_DEF(DigitSequence),
};

static TokenizerRules<Terminal> tokenizerRules{
    { "[ \t]+", Whitespace },
    { "(\r)?\n", NewLine },
    { "\\$", Dollar },
    { "\\/", ForwardSlash },
    { "=", Equals },
    { "\\.", Dot },
    { "\\{", CurlyBracketOpen },
    { "\\}", CurlyBracketClose },
    { "\"(?:[^\"\\\\]|\\\\.)*\"", String },
    { "[_a-zA-Z][_\\-a-zA-Z0-9]*", Name },
    { "#.*", Comment },
    { "[0-9]+", DigitSequence },
};

Lexer::Lexer(const std::filesystem::path& path, std::istream& stream)
    : m_tokenizer(path, stream)
{
    TokenDefinitions<Terminal> allTokenDefinitions{ tokenDefinitions };
    SetupTokenDefinitions(allTokenDefinitions);
    TokenizerRules<Terminal> allTokenizerRules{ tokenizerRules };
    SetTokenizerRules(allTokenizerRules);
}

parser::SourceLocation Lexer::GetCurrentLocation() const
{
    return m_tokenizer.GetCurrentLocation();
}

parser::Token<Terminal> Lexer::GetToken()
{
    return m_tokenizer.GetToken();
}

void Lexer::UngetToken(const parser::Token<Terminal>& token)
{
    m_tokenizer.UngetToken(token);
}

bool Lexer::IsAtEnd() const
{
    return m_tokenizer.IsAtEnd();
}

} // namespace expression
} // namespace cmake_parser
