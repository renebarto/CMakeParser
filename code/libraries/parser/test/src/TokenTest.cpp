#include "test-platform/GoogleTest.h"

#include "parser/Token.h"

namespace parser {

class TokenTest
    : public ::testing::Test
{
public:
    void SetUp() override
    {
        SetupTokenDefinitions<int>({
            { 0, "None" },
            { 1, "Whitespace" },
            });
    }
    void TearDown() override
    {
        SetupTokenDefinitions<int>({});
    }
};

TEST_F(TokenTest, ConstructDefault)
{
    Token<int> token;

    EXPECT_TRUE(token.IsNull());
    EXPECT_FALSE(token.IsInvalid());
    EXPECT_EQ(TokenType<int>{}, token.Type());
    EXPECT_EQ("", token.Value());
    EXPECT_EQ(SourceLocation{}, token.Location());
    EXPECT_EQ(SourceLocation{}, token.BeginLocation());
    EXPECT_EQ(SourceLocation{}, token.EndLocation());
    EXPECT_EQ("Null  (??-??)", token.Serialize());
}

TEST_F(TokenTest, Construct)
{
    SourceLocation beginLocation{ "ABC", 1, 1 };
    SourceLocation endLocation{ "ABC", 1, 4 };
    Token token(TokenType{ 1 }, "One", beginLocation, endLocation);

    EXPECT_FALSE(token.IsNull());
    EXPECT_FALSE(token.IsInvalid());
    EXPECT_EQ(TokenType{ 1 }, token.Type());
    EXPECT_EQ("One", token.Value());
    EXPECT_EQ(beginLocation, token.Location());
    EXPECT_EQ(beginLocation, token.BeginLocation());
    EXPECT_EQ(endLocation, token.EndLocation());
    EXPECT_EQ("Whitespace One (ABC(1:1)-ABC(1:4))", token.Serialize());
}

TEST_F(TokenTest, ConstructInvalid)
{
    SourceLocation beginLocation{ "ABC", 1, 1 };
    SourceLocation endLocation{ "ABC", 1, 4 };
    Token token(TokenType<int>::InvalidToken, "One", beginLocation, endLocation);

    EXPECT_FALSE(token.IsNull());
    EXPECT_TRUE(token.IsInvalid());
    EXPECT_EQ(TokenType<int>::InvalidToken, token.Type());
    EXPECT_EQ("One", token.Value());
    EXPECT_EQ(beginLocation, token.Location());
    EXPECT_EQ(beginLocation, token.BeginLocation());
    EXPECT_EQ(endLocation, token.EndLocation());
    EXPECT_EQ("Invalid One (ABC(1:1)-ABC(1:4))", token.Serialize());
}

TEST_F(TokenTest, StreamInsertionNarrow)
{
    SourceLocation beginLocation{ "ABC", 1, 1 };
    SourceLocation endLocation{ "ABC", 1, 4 };
    Token token(TokenType{ 1 }, "One", beginLocation, endLocation);

    std::ostringstream stream;
    stream << token;
    EXPECT_EQ("Whitespace One (ABC(1:1)-ABC(1:4))", stream.str());
}

} // namespace parser
