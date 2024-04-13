#include "parser/TokenizerRule.h"

#include "gtest/gtest.h"

namespace parser {

TEST(TokenizerRuleTest, ConstructDefault)
{
    TokenizerRule rule("", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches(""));
    EXPECT_FALSE(rule.Matches("abc"));
}

TEST(TokenizerRuleTest, ConstructSingleCharacter)
{
    TokenizerRule rule("a", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("a"));
    EXPECT_FALSE(rule.Matches("abc"));
}

TEST(TokenizerRuleTest, ConstructGreedyMatch)
{
    TokenizerRule rule("a+", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("a"));
    EXPECT_TRUE(rule.Matches("aa"));
    EXPECT_FALSE(rule.Matches("aab"));
}

TEST(TokenizerRuleTest, ConstructCommentBlock)
{
    TokenizerRule rule("/\\*[\\s\\S]*?\\*/", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("/**/"));
    EXPECT_TRUE(rule.Matches("/* Text */"));
    EXPECT_TRUE(rule.Matches("/* Text\r\non\r\nmultiple\r\nlines */"));
}

TEST(TokenizerRuleTest, ConstructIdentifier)
{
    TokenizerRule rule("[:alpha:][[:digit:][:alpha:]]*", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("a"));
    EXPECT_TRUE(rule.Matches("ab"));
    EXPECT_TRUE(rule.Matches("abc"));
    EXPECT_TRUE(rule.Matches("a0"));
    EXPECT_FALSE(rule.Matches("0a"));
}

TEST(TokenizerRuleTest, ConstructWhitespace)
{
    TokenizerRule rule("[[:space:]]+", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches(" "));
    EXPECT_TRUE(rule.Matches("\t"));
    EXPECT_TRUE(rule.Matches("\r"));
    EXPECT_TRUE(rule.Matches("\n"));
    EXPECT_TRUE(rule.Matches(" \t\r\n"));
    EXPECT_FALSE(rule.Matches("a"));
}

TEST(TokenizerRuleTest, ConstructStringWithQuotes)
{
    TokenizerRule rule("\"(\\\\\"|[^\"\\\\]*)*\"", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("\"\""));
    EXPECT_TRUE(rule.Matches("\"abc\""));
    EXPECT_TRUE(rule.Matches("\"\\\"\""));
}

TEST(TokenizerRuleTest, ConstructSingleLineComment)
{
    TokenizerRule rule("(?:/|$)(?:/|$).*", TokenType{});

    EXPECT_EQ(TokenType{}, rule.Type());
    EXPECT_TRUE(rule.Matches("//"));
    EXPECT_TRUE(rule.Matches("//abc"));
    EXPECT_TRUE(rule.Matches("/"));
}

} // namespace parser
