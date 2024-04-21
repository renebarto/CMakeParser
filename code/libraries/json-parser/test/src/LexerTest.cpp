#include "json-parser/Lexer.h"

#include "test-platform/GoogleTest.h"

using namespace parser;

namespace json_parser {

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

    EXPECT_EQ(Token<TokenTypes>{}, lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, Whitespace)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(" \t ");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " \t ", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 4)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, Brackets)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("[]{},:");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::SquareBracketOpen), "[", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 2)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::SquareBracketClose), "]", SourceLocation("ABC", 1, 2), SourceLocation("ABC", 1, 3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::CurlyBracketOpen), "{", SourceLocation("ABC", 1, 3), SourceLocation("ABC", 1, 4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::CurlyBracketClose), "}", SourceLocation("ABC", 1, 4), SourceLocation("ABC", 1, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Comma), ",", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Colon), ":", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 7)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, NullValue)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("null NULL Null");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::Null), "null", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Null), "NULL", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 10)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 10), SourceLocation("ABC", 1, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Null), "Null", SourceLocation("ABC", 1, 11), SourceLocation("ABC", 1, 15)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, BooleanValues)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("true false TrUe FaLsE");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::True), "true", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::False), "false", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 11), SourceLocation("ABC", 1, 12)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::True), "TrUe", SourceLocation("ABC", 1, 12), SourceLocation("ABC", 1, 16)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 16), SourceLocation("ABC", 1, 17)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::False), "FaLsE", SourceLocation("ABC", 1, 17), SourceLocation("ABC", 1, 22)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, NumberValues)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("1234 00123 123.456 123.456E+09");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::Number), "1234", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Number), "00123", SourceLocation("ABC", 1, 6), SourceLocation("ABC", 1, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 11), SourceLocation("ABC", 1, 12)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Number), "123.456", SourceLocation("ABC", 1, 12), SourceLocation("ABC", 1, 19)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 19), SourceLocation("ABC", 1, 20)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Number), "123.456", SourceLocation("ABC", 1, 20), SourceLocation("ABC", 1, 27)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::NumberExponent), "E+09", SourceLocation("ABC", 1, 27), SourceLocation("ABC", 1, 31)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, StringValues)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(R"("Text" "Some text with ' and \" quotes ")");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(TokenTypes::String), R"("Text")", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 7)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::Whitespace), " ", SourceLocation("ABC", 1, 7), SourceLocation("ABC", 1, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(TokenTypes::String), R"("Some text with ' and \" quotes ")", SourceLocation("ABC", 1, 8), SourceLocation("ABC", 1, 41)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

} // namespace json_parser
