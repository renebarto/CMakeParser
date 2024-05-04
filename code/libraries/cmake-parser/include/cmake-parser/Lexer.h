#pragma once

#include <filesystem>
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
    Minus,
    Plus,
    Colon,
    String,
    ParenthesisOpen,
    ParenthesisClose,
    CurlyBraceOpen,
    CurlyBraceClose,
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

public:
    Lexer(const std::filesystem::path& path, std::istream& stream);

    parser::SourceLocation GetCurrentLocation() const override;
    parser::Token<Terminal> GetToken() override;
    void UngetToken(const parser::Token<Terminal>& token) override;
    bool IsAtEnd() const override;
};

} // namespace cmake_parser
