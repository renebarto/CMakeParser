#include "json-parser/Lexer.h"

#include "parser/TokenizerRule.h"

using namespace json_parser;

#define TOKEN_DEF(x) { TokenTypes::x, #x }

static const parser::TokenDefinitions<TokenTypes> tokenDefinitions {
    TOKEN_DEF(None),
    TOKEN_DEF(EOS),
    TOKEN_DEF(Whitespace),
    TOKEN_DEF(SquareBracketOpen),
    TOKEN_DEF(SquareBracketClose),
    TOKEN_DEF(CurlyBraceOpen),
    TOKEN_DEF(CurlyBraceClose),
    TOKEN_DEF(Comma),
    TOKEN_DEF(Colon),
    TOKEN_DEF(Null),
    TOKEN_DEF(True),
    TOKEN_DEF(False),
    TOKEN_DEF(Number),
    TOKEN_DEF(NumberExponent),
    TOKEN_DEF(String),
};

static const parser::TokenizerRules<TokenTypes> tokenizerRules {
    { "[[:space:]]+", {TokenTypes::Whitespace} },
    { "\\[", TokenTypes::SquareBracketOpen},
    { "\\]", TokenTypes::SquareBracketClose },
    { "\\{", TokenTypes::CurlyBraceOpen},
    { "\\}", TokenTypes::CurlyBraceClose },
    { ",", TokenTypes::Comma },
    { ":", TokenTypes::Colon },
    { "[Nn][Uu][Ll][Ll]", TokenTypes::Null },
    { "[Tt][Rr][Uu][Ee]", TokenTypes::True },
    { "[Ff][Aa][Ll][Ss][Ee]", TokenTypes::False },
    { "[+-]?[0-9]*[.][0-9]+|[0-9]+[.]?", { TokenTypes::Number } },
    { "[Ee][+-]?[0-9]+", { TokenTypes::NumberExponent } },
    { "\"(?:[^\"\\\\]|\\\\.)*\"", { TokenTypes::String } },
};

Lexer::Lexer(const std::string& path, std::istream& stream)
    : m_tokenizer(path, stream)
    , m_tokens{}
{
    parser::TokenDefinitions<TokenTypes> allTokenDefinitions{ tokenDefinitions };
    SetupTokenDefinitions(allTokenDefinitions);
    parser::TokenizerRules<TokenTypes> allTokenizerRules{ tokenizerRules };
    SetTokenizerRules(allTokenizerRules);
}

parser::SourceLocation Lexer::GetCurrentLocation() const
{
    return m_tokenizer.GetCurrentLocation();
}

parser::Token<TokenTypes> Lexer::GetToken()
{
    return m_tokenizer.GetToken();
}

void Lexer::UngetToken(const parser::Token<TokenTypes>& token)
{
    m_tokenizer.UngetToken(token);
}

bool Lexer::IsAtEnd() const
{
    return m_tokenizer.IsAtEnd();
}

