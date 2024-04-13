#include "test-platform/GoogleTest.h"

#include "utility/StringFunctions.h"

namespace utility {

TEST(StringFunctionsTest, TrimDashesAndSpaces)
{
    std::string expected = "AB-CD";
    std::string actual = Trim("--- AB-CD ---", "- ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, TrimSpaces)
{
    std::string expected = "AB CD";
    std::string actual = TrimSpaces("   AB CD   ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, TrimLeftDashesAndSpaces)
{
    std::string expected = "AB CD ---";
    std::string actual = TrimLeft("--- AB CD ---", "- ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, TrimSpacesLeft)
{
    std::string expected = "AB CD   ";
    std::string actual = TrimSpacesLeft("   AB CD   ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, TrimRightDashesAndSpaces)
{
    std::string expected = "--- AB-CD";
    std::string actual = TrimRight("--- AB-CD ---", "- ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, TrimSpacesRight)
{
    std::string expected = "   AB CD";
    std::string actual = TrimSpacesRight("   AB CD   ");
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, IsEqual)
{
    std::string aString("Test");
    std::string aString2("Test");
    std::string aString3("TEST");
    std::string aString4("Test1");
    EXPECT_TRUE(IsEqual(aString, aString));
    EXPECT_TRUE(IsEqual(aString, aString2));
    EXPECT_FALSE(IsEqual(aString, aString3));
    EXPECT_FALSE(IsEqual(aString, aString4));
    EXPECT_TRUE(IsEqual(aString2, aString));
    EXPECT_FALSE(IsEqual(aString2, aString3));
    EXPECT_FALSE(IsEqual(aString2, aString4));
    EXPECT_FALSE(IsEqual(aString3, aString));
    EXPECT_FALSE(IsEqual(aString3, aString2));
    EXPECT_FALSE(IsEqual(aString3, aString4));
}

TEST(StringFunctionsTest, IsEqualIgnoreCase)
{
    std::string aString("Test");
    std::string aString2("Test");
    std::string aString3("TEST");
    std::string aString4("Test1");
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(IsEqualIgnoreCase(aString3, aString4));
}

TEST(StringFunctionsTest, ToLower)
{
    std::string input("Test");
    std::string expected("test");
    std::string actual = ToLower(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, ToUpper)
{
    std::string input("Test");
    std::string expected("TEST");
    std::string actual = ToUpper(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, QuoteUnquoted)
{
    std::string input("Test");
    std::string expected("\"Test\"");
    std::string actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, QuoteQuoted)
{
    std::string input("\"Test\"");
    std::string expected("\"Test\"");
    std::string actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, UnQuoteUnquoted)
{
    std::string input("Test");
    std::string expected("Test");
    std::string actual = UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, UnQuoteQuoted)
{
    std::string input("\"Test\"");
    std::string expected("Test");
    std::string actual = UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, SplitByCommas)
{
    std::string input("A,B,C,D");
    std::vector<std::string> expected {"A", "B", "C", "D"};
    auto actual = Split(input, ',');
    EXPECT_EQ(expected, actual);
}

TEST(StringFunctionsTest, SplitBySemicolonsLastEmpty)
{
    std::string input("A;B;C;");
    std::vector<std::string> expected{ "A", "B", "C", "" };
    auto actual = Split(input, ';');
    EXPECT_EQ(expected, actual);
}

} // namespace utility
