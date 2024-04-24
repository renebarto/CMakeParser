#include "utility/Regex.h"

#include "test-platform/GoogleTest.h"

using namespace utility;

TEST(RegexTest, VerifyMatchSimple)
{
    std::string regexString = "\\s*(\\w+)\\s*";
    std::string text = "  Hello  ";
    Matches matches;
    EXPECT_TRUE(VerifyMatch(text, regexString));
}

TEST(RegexTest, VerifyMatch)
{
    std::string regexString = "\\s*(\\w+)\\s*";
    std::string text = "  Hello  ";
    Matches matches;
    EXPECT_TRUE(VerifyMatch(text, matches, regexString));
    ASSERT_EQ(std::size_t {1}, matches.size());
    EXPECT_EQ("Hello", matches[0]);
}

TEST(RegexTest, VerifyMatches)
{
    std::string regexString = "\\s*(\\w+)\\s*";
    std::string text = "Hello World";
    std::vector<Matches> matches;
    EXPECT_TRUE(VerifyMatches(text, matches, regexString));
    ASSERT_EQ(std::size_t {2}, matches.size());
    ASSERT_EQ(std::size_t {1}, matches[0].size());
    EXPECT_EQ("Hello", matches[0][0]);
    ASSERT_EQ(std::size_t {1}, matches[1].size());
    EXPECT_EQ("World", matches[1][0]);
}

TEST(RegexTest, VerifyMatchFloatingPointIntegral)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("123", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("123", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointNoFraction)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("123.", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("123.", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointFractionOnly)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch(".123", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ(".123", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointWithFraction)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("123.456", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("123.456", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointPositive)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("+123.456", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("+123.456", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointNegative)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("-123.456", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("-123.456", matches[0]);
}

TEST(RegexTest, VerifyMatchFloatingPointScientific)
{
    std::string regexString = "([+-]?([0-9]*[.][0-9]+|[0-9]+[.]?)([Ee][+-]?[0-9]+)?)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("123.456E0", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("123.456E0", matches[0]);
}

TEST(RegexTest, VerifyMatchStringEmpty)
{
    std::string regexString = "(\\\"(\\\"|[^\\\"]*)*\\\")";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("\"\"", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("\"\"", matches[0]);
}

TEST(RegexTest, VerifyMatchStringNoInternalQuotes)
{
    std::string regexString = "(\\\"(\\\"|[^\\\"]*)*\\\")";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("\"ABC DEF\"", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("\"ABC DEF\"", matches[0]);
}

TEST(RegexTest, VerifyMatchStringWithInternalQuotes)
{
    std::string regexString = "(\\\"(\\\"|[^\"]*)*\\\")";
    Matches matches;
    EXPECT_TRUE(VerifyMatch("\"ABC \\\"DEF\\\"\"", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ("\"ABC \\\"DEF\\\"\"", matches[0]);
}

TEST(RegexTest, VerifyMatchWhitespace)
{
    std::string regexString = "(\\s*)";
    Matches matches;
    EXPECT_TRUE(VerifyMatch(" ", matches, regexString));
    ASSERT_LE(std::size_t{ 1 }, matches.size());
    EXPECT_EQ(" ", matches[0]);
}

