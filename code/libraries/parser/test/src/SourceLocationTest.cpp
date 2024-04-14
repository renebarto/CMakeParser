#include "parser/SourceLocation.h"

#include "test-platform/GoogleTest.h"

using namespace parser;

class SourceLocationTest
    : public ::testing::Test
{
public:
    std::string path;

    SourceLocationTest()
        : path{ "abc" }
    {
    }
};

TEST_F(SourceLocationTest, ConstructDefault)
{
    SourceLocation sourceLocation;
    EXPECT_EQ("", sourceLocation.UnitPath());
    EXPECT_EQ(0, sourceLocation.Line());
    EXPECT_EQ(0, sourceLocation.Column());
}

TEST_F(SourceLocationTest, ConstructPathOnly)
{
    SourceLocation sourceLocation(path);
    EXPECT_EQ(path, sourceLocation.UnitPath());
    EXPECT_EQ(0, sourceLocation.Line());
    EXPECT_EQ(0, sourceLocation.Column());
}

TEST_F(SourceLocationTest, Construct)
{
    SourceLocation sourceLocation(path, 1, 2);
    EXPECT_EQ(path, sourceLocation.UnitPath());
    EXPECT_EQ(1, sourceLocation.Line());
    EXPECT_EQ(2, sourceLocation.Column());
}

TEST_F(SourceLocationTest, NextCol)
{
    SourceLocation sourceLocation(path, 1, 2);
    sourceLocation.NextCol();
    EXPECT_EQ(1, sourceLocation.Line());
    EXPECT_EQ(3, sourceLocation.Column());
}

TEST_F(SourceLocationTest, NextLine)
{
    SourceLocation sourceLocation(path, 1, 2);
    sourceLocation.NextLine();
    EXPECT_EQ(2, sourceLocation.Line());
    EXPECT_EQ(1, sourceLocation.Column());
}

TEST_F(SourceLocationTest, ResetLine)
{
    SourceLocation sourceLocation(path, 1, 2);
    sourceLocation.ResetLine();
    EXPECT_EQ(1, sourceLocation.Line());
    EXPECT_EQ(1, sourceLocation.Column());
}

TEST_F(SourceLocationTest, IsValid)
{
    SourceLocation sourceLocation;
    EXPECT_FALSE(sourceLocation.IsValid());
    sourceLocation.NextLine();
    EXPECT_TRUE(sourceLocation.IsValid());
}

TEST_F(SourceLocationTest, Serialize)
{
    SourceLocation sourceLocation(path, 2, 3);
    EXPECT_EQ("abc(2:3)", sourceLocation.Serialize());
}
