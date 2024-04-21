#include "test-platform/GoogleTest.h"

#include "parser/TokenType.h"

namespace parser {

TEST(TokenTypeTest, TokenTypeToString)
{
    SetupTokenDefinitions<int>({
        { 0, "None" },
        { 1, "Whitespace" },
        });
    EXPECT_EQ("None", TokenType{ 0 }.Serialize());
    EXPECT_EQ("Whitespace", TokenType{ 1 }.Serialize());
}

TEST(TokenTypeTest, StringToTokenType)
{
    SetupTokenDefinitions<int>({
        { 0, "None" },
        { 1, "Whitespace" },
        });
    EXPECT_EQ(TokenType<int>{ 0 }, StringToTokenType<int>("None"));
    EXPECT_EQ(TokenType<int>{ 1 }, StringToTokenType<int>("Whitespace"));
}

TEST(TokenTypeTest, InsertionOperator)
{
    std::ostringstream stream;
    stream << TokenType{ 0 } << " " << TokenType{ 1 };
    EXPECT_EQ("None Whitespace", stream.str());
}

} // namespace parser
