#include "parser/Reader.h"

#include "gtest/gtest.h"

using namespace parser;

TEST(ReaderTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream;
    Reader reader(compilationUnit, stream);

    char ch;
    EXPECT_FALSE(reader.EndOfStream());
    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), reader.GetLocation());
    EXPECT_TRUE(reader.EndOfStream());
}

TEST(ReaderTest, NonEmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("1\n2\r3\n\r");
    Reader reader(compilationUnit, stream);

    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), reader.GetLocation());

    char ch;
    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('1', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('2', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('3', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_EQ(char{}, ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_TRUE(reader.EndOfStream());
}

TEST(ReaderTest, RestoreChars)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("1\n2\r3\n\r");
    Reader reader(compilationUnit, stream);

    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), reader.GetLocation());

    char ch;
    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('1', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 2), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());

    reader.RestoreChars("\n", SourceLocation(compilationUnit, 1, 2));
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 2), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('2', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('3', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    reader.RestoreChars("2\r3", SourceLocation(compilationUnit, 2, 1));
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('2', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('3', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 2, 2), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_EQ(char{}, ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_TRUE(reader.EndOfStream());

    reader.RestoreChars("\r", SourceLocation(compilationUnit, 3, 1));
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('\r', ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_FALSE(reader.EndOfStream());

    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_EQ(char{}, ch);
    EXPECT_EQ(SourceLocation(compilationUnit, 3, 1), reader.GetLocation());
    EXPECT_TRUE(reader.EndOfStream());
}
