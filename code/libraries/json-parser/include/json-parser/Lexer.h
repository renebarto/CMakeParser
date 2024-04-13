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
    CurlyBracketOpen,
    CurlyBracketClose,
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
{
private:
    parser::Tokenizer m_tokenizer;
    parser::TokenList m_tokens;

public:
    Lexer(const std::string& path, std::istream& stream);

    parser::SourceLocation GetCurrentLocation() const;
    parser::Token GetToken();
    void UngetToken(const parser::Token& token);
    bool IsAtEnd() const;
};

} // namespace json_parser
