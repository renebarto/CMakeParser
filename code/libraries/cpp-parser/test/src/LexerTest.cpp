#include "cpp-parser/Lexer.h"

#include "test-platform/GoogleTest.h"

namespace parser {

class LexerTest
    : public ::testing::Test
{
public:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(LexerTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("");
    Lexer lexer(compilationUnit, stream);

    EXPECT_TRUE(lexer.Parse());
}

TEST_F(LexerTest, SingleLineComment)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("// Comment");
    Lexer lexer(compilationUnit, stream);

    EXPECT_TRUE(lexer.Parse());
    ASSERT_EQ(size_t{ 1 }, lexer.GetTokens().size());
    EXPECT_EQ(Token(TokenType(SingleLineComment), "// Comment", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 11)), lexer.GetTokens()[0]);
}

TEST_F(LexerTest, SingleLineCommentNewLine)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("// Comment\n\r\n");
    Lexer lexer(compilationUnit, stream);

    EXPECT_TRUE(lexer.Parse());
    ASSERT_EQ(size_t{ 3 }, lexer.GetTokens().size());
    EXPECT_EQ(Token(TokenType(SingleLineComment), "// Comment", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 11)), lexer.GetTokens()[0]);
    EXPECT_EQ(Token(TokenType(NewLine), "\n", SourceLocation("ABC", 1, 11), SourceLocation("ABC", 2, 1)), lexer.GetTokens()[1]);
    EXPECT_EQ(Token(TokenType(NewLine), "\r\n", SourceLocation("ABC", 2, 1), SourceLocation("ABC", 3, 1)), lexer.GetTokens()[2]);
}

TEST_F(LexerTest, PunctationMarks)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("~!@#$%^&*()+`-={}|[]\\:\";'<>?,./");
    Lexer lexer(compilationUnit, stream);

    EXPECT_TRUE(lexer.Parse());
    ASSERT_EQ(size_t{ 31 }, lexer.GetTokens().size());
    EXPECT_EQ(Token(TokenType(Tilde),               "~", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 2)), lexer.GetTokens()[0]);
    EXPECT_EQ(Token(TokenType(ExclamationMark),     "!", SourceLocation("ABC", 1, 2), SourceLocation("ABC", 1, 3)), lexer.GetTokens()[1]);
    EXPECT_EQ(Token(TokenType(At),                  "@", SourceLocation("ABC", 1, 3), SourceLocation("ABC", 1, 4)), lexer.GetTokens()[2]);
    EXPECT_EQ(Token(TokenType(Hash),                "#", SourceLocation("ABC", 1, 4), SourceLocation("ABC", 1, 5)), lexer.GetTokens()[3]);
    EXPECT_EQ(Token(TokenType(Dollar),              "$", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetTokens()[4]);
    EXPECT_EQ(Token(TokenType(Percent),             "%", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 7)), lexer.GetTokens()[5]);
    EXPECT_EQ(Token(TokenType(Circumflex),          "^", SourceLocation("ABC", 1, 7), SourceLocation("ABC", 1, 8)), lexer.GetTokens()[6]);
    EXPECT_EQ(Token(TokenType(Ampersand),           "&", SourceLocation("ABC", 1, 8), SourceLocation("ABC", 1, 9)), lexer.GetTokens()[7]);
    EXPECT_EQ(Token(TokenType(Asterisk),            "*", SourceLocation("ABC", 1, 9), SourceLocation("ABC", 1, 10)), lexer.GetTokens()[8]);
    EXPECT_EQ(Token(TokenType(ParenthesisOpen),     "(", SourceLocation("ABC", 1, 10), SourceLocation("ABC", 1, 11)), lexer.GetTokens()[9]);
    EXPECT_EQ(Token(TokenType(ParenthesisClose),    ")", SourceLocation("ABC", 1, 11), SourceLocation("ABC", 1, 12)), lexer.GetTokens()[10]);
    EXPECT_EQ(Token(TokenType(Plus),                "+", SourceLocation("ABC", 1, 12), SourceLocation("ABC", 1, 13)), lexer.GetTokens()[11]);
    EXPECT_EQ(Token(TokenType(BackQuote),           "`", SourceLocation("ABC", 1, 13), SourceLocation("ABC", 1, 14)), lexer.GetTokens()[12]);
    EXPECT_EQ(Token(TokenType(Minus),               "-", SourceLocation("ABC", 1, 14), SourceLocation("ABC", 1, 15)), lexer.GetTokens()[13]);
    EXPECT_EQ(Token(TokenType(Equals),              "=", SourceLocation("ABC", 1, 15), SourceLocation("ABC", 1, 16)), lexer.GetTokens()[14]);
    EXPECT_EQ(Token(TokenType(CurlyBracketOpen),    "{", SourceLocation("ABC", 1, 16), SourceLocation("ABC", 1, 17)), lexer.GetTokens()[15]);
    EXPECT_EQ(Token(TokenType(CurlyBracketClose),   "}", SourceLocation("ABC", 1, 17), SourceLocation("ABC", 1, 18)), lexer.GetTokens()[16]);
    EXPECT_EQ(Token(TokenType(Pipe),                "|", SourceLocation("ABC", 1, 18), SourceLocation("ABC", 1, 19)), lexer.GetTokens()[17]);
    EXPECT_EQ(Token(TokenType(SquareBracketOpen),   "[", SourceLocation("ABC", 1, 19), SourceLocation("ABC", 1, 20)), lexer.GetTokens()[18]);
    EXPECT_EQ(Token(TokenType(SquareBracketClose),  "]", SourceLocation("ABC", 1, 20), SourceLocation("ABC", 1, 21)), lexer.GetTokens()[19]);
    EXPECT_EQ(Token(TokenType(Backslash),           "\\", SourceLocation("ABC", 1, 21), SourceLocation("ABC", 1, 22)), lexer.GetTokens()[20]);
    EXPECT_EQ(Token(TokenType(Colon),               ":", SourceLocation("ABC", 1, 22), SourceLocation("ABC", 1, 23)), lexer.GetTokens()[21]);
    EXPECT_EQ(Token(TokenType(DoubleQuote),         "\"", SourceLocation("ABC", 1, 23), SourceLocation("ABC", 1, 24)), lexer.GetTokens()[22]);
    EXPECT_EQ(Token(TokenType(Semicolon),           ";", SourceLocation("ABC", 1, 24), SourceLocation("ABC", 1, 25)), lexer.GetTokens()[23]);
    EXPECT_EQ(Token(TokenType(SingleQuote),         "'", SourceLocation("ABC", 1, 25), SourceLocation("ABC", 1, 26)), lexer.GetTokens()[24]);
    EXPECT_EQ(Token(TokenType(AngleBracketOpen),    "<", SourceLocation("ABC", 1, 26), SourceLocation("ABC", 1, 27)), lexer.GetTokens()[25]);
    EXPECT_EQ(Token(TokenType(AngleBracketClose),   ">", SourceLocation("ABC", 1, 27), SourceLocation("ABC", 1, 28)), lexer.GetTokens()[26]);
    EXPECT_EQ(Token(TokenType(QuestionMark),        "?", SourceLocation("ABC", 1, 28), SourceLocation("ABC", 1, 29)), lexer.GetTokens()[27]);
    EXPECT_EQ(Token(TokenType(Comma),               ",", SourceLocation("ABC", 1, 29), SourceLocation("ABC", 1, 30)), lexer.GetTokens()[28]);
    EXPECT_EQ(Token(TokenType(Period),              ".", SourceLocation("ABC", 1, 30), SourceLocation("ABC", 1, 31)), lexer.GetTokens()[29]);
    EXPECT_EQ(Token(TokenType(ForwardSlash),        "/", SourceLocation("ABC", 1, 31), SourceLocation("ABC", 1, 32)), lexer.GetTokens()[30]);
}

TEST_F(LexerTest, ReservedKeywords)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("class struct virtual override final char int unsigned long float double");
    Lexer lexer(compilationUnit, stream);

    EXPECT_TRUE(lexer.Parse());
    ASSERT_EQ(size_t{ 21 }, lexer.GetTokens().size());
    EXPECT_EQ(Token(TokenType(ClassKeyword), "class", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 6)), lexer.GetTokens()[0]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 7)), lexer.GetTokens()[1]);
    EXPECT_EQ(Token(TokenType(StructKeyword), "struct", SourceLocation("ABC", 1, 7), SourceLocation("ABC", 1, 13)), lexer.GetTokens()[2]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 13), SourceLocation("ABC", 1, 14)), lexer.GetTokens()[3]);
    EXPECT_EQ(Token(TokenType(VirtualKeyword), "virtual", SourceLocation("ABC", 1, 14), SourceLocation("ABC", 1, 21)), lexer.GetTokens()[4]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 21), SourceLocation("ABC", 1, 22)), lexer.GetTokens()[5]);
    EXPECT_EQ(Token(TokenType(OverrideKeyword), "override", SourceLocation("ABC", 1, 22), SourceLocation("ABC", 1, 30)), lexer.GetTokens()[6]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 30), SourceLocation("ABC", 1, 31)), lexer.GetTokens()[7]);
    EXPECT_EQ(Token(TokenType(FinalKeyword), "final", SourceLocation("ABC", 1, 31), SourceLocation("ABC", 1, 36)), lexer.GetTokens()[8]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 36), SourceLocation("ABC", 1, 37)), lexer.GetTokens()[9]);
    EXPECT_EQ(Token(TokenType(CharKeyword), "char", SourceLocation("ABC", 1, 37), SourceLocation("ABC", 1, 41)), lexer.GetTokens()[10]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 41), SourceLocation("ABC", 1, 42)), lexer.GetTokens()[11]);
    EXPECT_EQ(Token(TokenType(IntKeyword), "int", SourceLocation("ABC", 1, 42), SourceLocation("ABC", 1, 45)), lexer.GetTokens()[12]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 45), SourceLocation("ABC", 1, 46)), lexer.GetTokens()[13]);
    EXPECT_EQ(Token(TokenType(UnsignedKeyword), "unsigned", SourceLocation("ABC", 1, 46), SourceLocation("ABC", 1, 54)), lexer.GetTokens()[14]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 54), SourceLocation("ABC", 1, 55)), lexer.GetTokens()[15]);
    EXPECT_EQ(Token(TokenType(LongKeyword), "long", SourceLocation("ABC", 1, 55), SourceLocation("ABC", 1, 59)), lexer.GetTokens()[16]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 59), SourceLocation("ABC", 1, 60)), lexer.GetTokens()[17]);
    EXPECT_EQ(Token(TokenType(FloatKeyword), "float", SourceLocation("ABC", 1, 60), SourceLocation("ABC", 1, 65)), lexer.GetTokens()[18]);
    EXPECT_EQ(Token(TokenType(Whitespace), " ", SourceLocation("ABC", 1, 65), SourceLocation("ABC", 1, 66)), lexer.GetTokens()[19]);
    EXPECT_EQ(Token(TokenType(DoubleKeyword), "double", SourceLocation("ABC", 1, 66), SourceLocation("ABC", 1, 72)), lexer.GetTokens()[20]);
}

} // namespace parser
