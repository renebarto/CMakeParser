#pragma once

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
    String,
    ParenthesisOpen,
    ParenthesisClose,
    CurlyBracketOpen,
    CurlyBracketClose,
    VersionNumber,
    Identifier,
    ProjectName,
    Comment,
    CompilerOption,
    HexNumber,
    Number,

    AddSubDirectoryKeyword,
    CMakeMinimumRequiredKeyword,
    ElseKeyword,
    EndForEachKeyword,
    EndIfKeyword,
    FindPackageKeyword,
    ForEachKeyword,
    IfKeyword,
    IncludeKeyword,
    ListKeyword,
    MessageKeyword,
    OptionKeyword,
    ProjectKeyword,
    SetKeyword,
    StringKeyword,
    VersionKeyword,
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

} // namespace cmake_parser
