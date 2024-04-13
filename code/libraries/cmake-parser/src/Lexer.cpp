#include "cmake-parser/Lexer.h"

#include "parser/TokenizerRule.h"

using namespace parser;

namespace cmake_parser {

#define TOKEN_DEF(x) { Terminal::x, #x }
static TokenDefinitions reservedKeywords {
    TOKEN_DEF(AddSubDirectoryKeyword),
    TOKEN_DEF(CMakeMinimumRequiredKeyword),
    TOKEN_DEF(ElseKeyword),
    TOKEN_DEF(EndForEachKeyword),
    TOKEN_DEF(EndIfKeyword),
    TOKEN_DEF(FindPackageKeyword),
    TOKEN_DEF(ForEachKeyword),
    TOKEN_DEF(IfKeyword),
    TOKEN_DEF(IncludeKeyword),
    TOKEN_DEF(ListKeyword),
    TOKEN_DEF(MessageKeyword),
    TOKEN_DEF(OptionKeyword),
    TOKEN_DEF(ProjectKeyword),
    TOKEN_DEF(SetKeyword),
    TOKEN_DEF(StringKeyword),
    TOKEN_DEF(VersionKeyword),
};
static TokenDefinitions tokenDefinitions {
    TOKEN_DEF(None),
    TOKEN_DEF(Whitespace),
    TOKEN_DEF(NewLine),
    TOKEN_DEF(Dollar),
    TOKEN_DEF(ForwardSlash),
    TOKEN_DEF(Equals),
    TOKEN_DEF(String),
    TOKEN_DEF(ParenthesisOpen),
    TOKEN_DEF(ParenthesisClose),
    TOKEN_DEF(CurlyBracketOpen),
    TOKEN_DEF(CurlyBracketClose),
    TOKEN_DEF(VersionNumber),
    TOKEN_DEF(Identifier),
    TOKEN_DEF(ProjectName),
    TOKEN_DEF(Comment),
    TOKEN_DEF(CompilerOption),
    TOKEN_DEF(HexNumber),
    TOKEN_DEF(Number),
};

static TokenizerRules reservedKeywordRules {
    { "add_subdirectory", AddSubDirectoryKeyword, true },
    { "cmake_minimum_required", CMakeMinimumRequiredKeyword, true },
    { "else", ElseKeyword, true },
    { "endforeach", EndForEachKeyword, true },
    { "endif", EndIfKeyword, true },
    { "find_package", FindPackageKeyword, true },
    { "foreach", ForEachKeyword, true },
    { "if", IfKeyword, true },
    { "include", IncludeKeyword, true },
    { "list", ListKeyword, true },
    { "message", MessageKeyword, true },
    { "option", OptionKeyword, true },
    { "project", ProjectKeyword, true },
    { "set", SetKeyword, true },
    { "string", StringKeyword, true },
    { "version", VersionKeyword, true },
};
static TokenizerRules tokenizerRules{
    { "[ \t]+", Whitespace },
    { "(\r)?\n", NewLine },
    { "\\$", Dollar },
    { "\\/", ForwardSlash },
    { "=", Equals },
    { "\\(", ParenthesisOpen },
    { "\\)", ParenthesisClose },
    { "\\{", CurlyBracketOpen },
    { "\\}", CurlyBracketClose },
    { "\"(?:[^\"\\\\]|\\\\.)*\"", String },
    { "[[:digit:]]\\.[[:digit:]](\\.[[:digit:]])?", VersionNumber },
    { "[_a-zA-Z][_a-zA-Z0-9]*", Identifier },
    { "[_a-zA-Z][_\\-a-zA-Z0-9]*", ProjectName },
    { "#.*", Comment },
    { "[\\/\\-][a-zA-Z].*", CompilerOption },
    { "0[xX][0-9a-fA-F]+", HexNumber },
    { "[0-9]+", Number },
};

Lexer::Lexer(const std::string& path, std::istream& stream)
    : m_tokenizer(path, stream)
    , m_tokens{}
{
    TokenDefinitions allTokenDefinitions{ reservedKeywords };
    allTokenDefinitions.insert(allTokenDefinitions.end(), tokenDefinitions.begin(), tokenDefinitions.end());
    SetupTokenDefinitions(allTokenDefinitions);
    TokenizerRules allTokenizerRules{ reservedKeywordRules };
    allTokenizerRules.insert(allTokenizerRules.end(), tokenizerRules.begin(), tokenizerRules.end());
    SetTokenizerRules(allTokenizerRules);
}

parser::SourceLocation Lexer::GetCurrentLocation() const
{
    return m_tokenizer.GetCurrentLocation();
}

parser::Token Lexer::GetToken()
{
    return m_tokenizer.GetToken();
}

void Lexer::UngetToken(const parser::Token& token)
{
    m_tokenizer.UngetToken(token);
}

bool Lexer::IsAtEnd() const
{
    return m_tokenizer.IsAtEnd();
}

} // namespace cmake_parser
