#pragma once

#include <set>
#include "parser/Tokenizer.h"

namespace cmake_parser {

enum Terminal
{
    None,
    Whitespace,
    NewLine,
    Dollar,
    ForwardSlash,
    Equals,
    Dot,
    String,
    ParenthesisOpen,
    ParenthesisClose,
    CurlyBracketOpen,
    CurlyBracketClose,
    Identifier,
    Name,
    Comment,
    //CompilerOption,
    //HexNumber,
    //Number,
    DigitSequence,
};
using TerminalSet = std::set<Terminal>;

class Lexer
    : public parser::ITokenizer<Terminal>
{
private:
    parser::Tokenizer<Terminal> m_tokenizer;
    parser::TokenList<Terminal> m_tokens;

public:
    Lexer(const std::string& path, std::istream& stream);

    parser::SourceLocation GetCurrentLocation() const override;
    parser::Token<Terminal> GetToken() override;
    void UngetToken(const parser::Token<Terminal>& token) override;
    bool IsAtEnd() const override;
};

} // namespace cmake_parser
