#include "test-platform/GoogleTest.h"

#include "parser/TokenType.h"

namespace parser {

TEST(TokenTypeTest, TokenTypeToString)
{
    SetupTokenDefinitions({
        { 0, "None" },
        { 1, "Whitespace" },
        });
    EXPECT_EQ("None", TokenType{ 0 }.Serialize());
    EXPECT_EQ("Whitespace", TokenType{ 1 }.Serialize());
}

TEST(TokenTypeTest, StringToTokenType)
{
    SetupTokenDefinitions({
        { 0, "None" },
        { 1, "Whitespace" },
        });
    EXPECT_EQ(TokenType{ 0 }, StringToTokenType("None"));
    EXPECT_EQ(TokenType{ 1 }, StringToTokenType("Whitespace"));
}

TEST(TokenTypeTest, InsertionOperator)
{
    std::ostringstream stream;
    stream << TokenType{ 0 } << " " << TokenType{ 1 };
    EXPECT_EQ("None Whitespace", stream.str());
}

} // namespace parser
