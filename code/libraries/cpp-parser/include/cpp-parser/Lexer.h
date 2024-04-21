#pragma once

#include "parser/Tokenizer.h"

namespace parser {

enum TokenTypes
{
    None,
    Whitespace,
    NewLine,
    SingleLineComment,
    Tilde,
    ExclamationMark,
    At,
    Hash,
    Dollar,
    Percent,
    Circumflex,
    Ampersand,
    Asterisk,
    ParenthesisOpen,
    ParenthesisClose,
    Plus,
    BackQuote,
    Minus,
    Equals,
    CurlyBracketOpen,
    CurlyBracketClose,
    Pipe,
    SquareBracketOpen,
    SquareBracketClose,
    Backslash,
    Colon,
    DoubleQuote,
    Semicolon,
    SingleQuote,
    AngleBracketOpen,
    AngleBracketClose,
    QuestionMark,
    Comma,
    Period,
    ForwardSlash,
    ClassKeyword,
    StructKeyword,
    VirtualKeyword,
    OverrideKeyword,
    FinalKeyword,
    CharKeyword,
    IntKeyword,
    UnsignedKeyword,
    LongKeyword,
    FloatKeyword,
    DoubleKeyword,
};

class Lexer
{
private:
    Tokenizer<TokenTypes> m_tokenizer;
    TokenList<TokenTypes> m_tokens;

public:
    Lexer(const std::string& path, std::istream& stream);
    bool Parse();

    TokenList<TokenTypes>& GetTokens();
};

} // namespace parser
