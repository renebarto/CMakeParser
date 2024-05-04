#pragma once

#include "parser/Tokenizer.h"

namespace json_parser {

enum TokenTypes
{
    None,
    EOS,
    Whitespace,
    SquareBracketOpen,
    SquareBracketClose,
    CurlyBraceOpen,
    CurlyBraceClose,
    Comma,
    Colon,
    Null,
    True,
    False,
    Number,
    NumberExponent,
    String,
};

class Lexer
    : public parser::ITokenizer<TokenTypes>
{
private:
    parser::Tokenizer<TokenTypes> m_tokenizer;
    parser::TokenList<TokenTypes> m_tokens;

public:
    Lexer(const std::string& path, std::istream& stream);

    parser::SourceLocation GetCurrentLocation() const override;
    parser::Token<TokenTypes> GetToken() override;
    void UngetToken(const parser::Token<TokenTypes>& token) override;
    bool IsAtEnd() const override;
};

} // namespace json_parser
