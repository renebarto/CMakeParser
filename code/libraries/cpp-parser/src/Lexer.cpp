#include "cpp-parser/Lexer.h"

#include "parser/TokenizerRule.h"

namespace parser {

#define TOKEN_DEF(x) { TokenTypes::x, #x }
static TokenDefinitions reservedKeywords {
    TOKEN_DEF(ClassKeyword),
    TOKEN_DEF(StructKeyword),
    TOKEN_DEF(VirtualKeyword),
    TOKEN_DEF(OverrideKeyword),
    TOKEN_DEF(FinalKeyword),
    TOKEN_DEF(CharKeyword),
    TOKEN_DEF(IntKeyword),
    TOKEN_DEF(UnsignedKeyword),
    TOKEN_DEF(LongKeyword),
    TOKEN_DEF(FloatKeyword),
    TOKEN_DEF(DoubleKeyword),
};
static TokenDefinitions tokenDefinitions {
    TOKEN_DEF(Whitespace),
    TOKEN_DEF(NewLine),
    TOKEN_DEF(SingleLineComment),
    TOKEN_DEF(Tilde),
    TOKEN_DEF(ExclamationMark),
    TOKEN_DEF(At),
    TOKEN_DEF(Hash),
    TOKEN_DEF(Dollar),
    TOKEN_DEF(Percent),
    TOKEN_DEF(Circumflex),
    TOKEN_DEF(Ampersand),
    TOKEN_DEF(Asterisk),
    TOKEN_DEF(ParenthesisOpen),
    TOKEN_DEF(ParenthesisClose),
    TOKEN_DEF(Plus),
    TOKEN_DEF(BackQuote),
    TOKEN_DEF(Minus),
    TOKEN_DEF(Equals),
    TOKEN_DEF(CurlyBracketOpen),
    TOKEN_DEF(CurlyBracketClose),
    TOKEN_DEF(Pipe),
    TOKEN_DEF(SquareBracketOpen),
    TOKEN_DEF(SquareBracketClose),
    TOKEN_DEF(Backslash),
    TOKEN_DEF(Colon),
    TOKEN_DEF(DoubleQuote),
    TOKEN_DEF(Semicolon),
    TOKEN_DEF(SingleQuote),
    TOKEN_DEF(AngleBracketOpen),
    TOKEN_DEF(AngleBracketClose),
    TOKEN_DEF(QuestionMark),
    TOKEN_DEF(Comma),
    TOKEN_DEF(Period),
    TOKEN_DEF(ForwardSlash),    
};

static TokenizerRules reservedKeywordRules{
    { "class", ClassKeyword},
    { "struct", StructKeyword},
    { "virtual", VirtualKeyword},
    { "override", OverrideKeyword},
    { "final", FinalKeyword},
    { "char", CharKeyword},
    { "int", IntKeyword},
    { "unsigned", UnsignedKeyword},
    { "long", LongKeyword},
    { "float", FloatKeyword},
    { "double", DoubleKeyword},
};
static TokenizerRules tokenizerRules{
    { "[ \t]+", Whitespace},
    { "(\r)?\n", NewLine},
    { "~", Tilde},
    { "!", ExclamationMark},
    { "@", At},
    { "#", Hash},
    { "\\$", Dollar},
    { "%", Percent},
    { "\\^", Circumflex},
    { "&", Ampersand},
    { "\\*", Asterisk},
    { "\\(", ParenthesisOpen},
    { "\\)", ParenthesisClose},
    { "\\+", Plus},
    { "`", BackQuote},
    { "-", Minus},
    { "=", Equals},
    { "\\{", CurlyBracketOpen},
    { "\\}", CurlyBracketClose},
    { "\\|", Pipe},
    { "\\[", SquareBracketOpen},
    { "\\]", SquareBracketClose},
    { "\\\\", Backslash},
    { ":", Colon},
    { "\"", DoubleQuote},
    { ";", Semicolon},
    { "'", SingleQuote},
    { "<", AngleBracketOpen},
    { ">", AngleBracketClose},
    { "\\?", QuestionMark},
    { ",", Comma},
    { "\\.", Period},
    { "/", ForwardSlash},    
    { "(?:/|$)(?:/|$).*$", SingleLineComment},
};

Lexer::Lexer(const std::string& path, std::istream& stream)
    : m_tokenizer(path, stream)
    , m_tokens{}
{
}

bool Lexer::Parse()
{
    TokenDefinitions allTokenDefinitions{ tokenDefinitions };
    allTokenDefinitions.insert(allTokenDefinitions.end(), reservedKeywords.begin(), reservedKeywords.end());
    SetupTokenDefinitions(allTokenDefinitions);
    TokenizerRules allTokenizerRules{ tokenizerRules };
    allTokenizerRules.insert(allTokenizerRules.end(), reservedKeywordRules.begin(), reservedKeywordRules.end());
    SetTokenizerRules(allTokenizerRules);
    bool result{ true };
    while (!m_tokenizer.IsAtEnd())
    {
        auto token = m_tokenizer.GetToken();
        if (token.IsInvalid() || (token.IsNull() && !m_tokenizer.IsAtEnd()))
        {
            result = false;
            break;
        }
        if (!token.IsNull())
            m_tokens.push_back(token);
    }

    return result;
}

TokenList& Lexer::GetTokens()
{
    return m_tokens;
}

} // namespace parser
