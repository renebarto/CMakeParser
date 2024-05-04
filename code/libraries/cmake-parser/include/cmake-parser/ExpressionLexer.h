#pragma once

#include <filesystem>
#include <set>
#include "parser/Tokenizer.h"

namespace cmake_parser {
namespace expression {

enum Terminal
{
    None,
    Whitespace,
    NewLine,
    Dollar,
    ForwardSlash,
    Equals,
    Dot,
    SemiColon,
    String,
    CurlyBraceOpen,
    CurlyBraceClose,
    Name,
    Comment,
    DigitSequence,
};

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

} // namespace expression
} // namespace cmake_parser
