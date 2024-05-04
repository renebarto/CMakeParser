#include "cmake-parser/Lexer.h"

#include "parser/TokenizerRule.h"

using namespace parser;

namespace cmake_parser {

#define TOKEN_DEF(x) { Terminal::x, #x }
static TokenDefinitions<Terminal> tokenDefinitions {
    TOKEN_DEF(None),
    TOKEN_DEF(Whitespace),
    TOKEN_DEF(NewLine),
    TOKEN_DEF(Dollar),
    TOKEN_DEF(ForwardSlash),
    TOKEN_DEF(Equals),
    TOKEN_DEF(Dot),
    TOKEN_DEF(Minus),
    TOKEN_DEF(Plus),
    TOKEN_DEF(Colon),
    TOKEN_DEF(String),
    TOKEN_DEF(ParenthesisOpen),
    TOKEN_DEF(ParenthesisClose),
    TOKEN_DEF(CurlyBraceOpen),
    TOKEN_DEF(CurlyBraceClose),
    TOKEN_DEF(Identifier),
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
    { "\\-", Minus },
    { "\\+", Plus },
    { "\\:", Plus },
    { "\\(", ParenthesisOpen },
    { "\\)", ParenthesisClose },
    { "\\{", CurlyBraceOpen },
    { "\\}", CurlyBraceClose },
    { "\"(?:[^\"\\\\]|\\\\.)*\"", String },
    //{ "[[:digit:]]\\.[[:digit:]](\\.[[:digit:]])?", VersionNumber },
    { "[_a-zA-Z][_a-zA-Z0-9]*", Identifier },
    { "[_a-zA-Z][_\\-a-zA-Z0-9]*", Name },
    { "#.*", Comment },
    //{ "[\\/\\-][a-zA-Z].*", CompilerOption },
    //{ "0[xX][0-9a-fA-F]+", HexNumber },
    //{ "[0-9]+", Number },
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

} // namespace cmake_parser
