#include "cpp-parser/Parser.h"

#include "gtest/gtest.h"

namespace parser {

class ParserTest
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

TEST_F(ParserTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

//TEST_F(ParserTest, SingleLineComment)
//{
//    std::string compilationUnit("ABC");
//    std::istringstream stream("// Comment");
//    Parser parser(compilationUnit, stream);
//
//    EXPECT_TRUE(parser.Parse());
//}
//
//TEST_F(ParserTest, Keywords)
//{
//    std::string compilationUnit("ABC");
//    std::istringstream stream("class");
//    Parser parser(compilationUnit, stream);
//
//    EXPECT_TRUE(parser.Parse());
//}

} // namespace parser
