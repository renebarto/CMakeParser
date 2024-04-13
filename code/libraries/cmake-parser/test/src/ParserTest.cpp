#include "cmake-parser/Parser.h"

#include "gtest/gtest.h"

namespace cmake_parser {

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

} // namespace cmake_parser
