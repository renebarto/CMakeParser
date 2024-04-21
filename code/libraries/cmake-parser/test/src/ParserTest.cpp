#include "cmake-parser/Parser.h"

#include "test-platform/GoogleTest.h"
#include <filesystem>
#include <fstream>

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

TEST_F(ParserTest, MinimalCMakeFile)
{
    std::string compilationUnit("CMakeLists_minimal.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

TEST_F(ParserTest, MinimalCMakeProject)
{
    std::string compilationUnit("CMakeLists_minimal_project.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

TEST_F(ParserTest, MinimalCMakeProjectWithMessage)
{
    std::string compilationUnit("CMakeLists_minimal_project_message.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

TEST_F(ParserTest, MinimalCMakeProjectWithSet)
{
    std::string compilationUnit("CMakeLists_minimal_project_set.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

} // namespace cmake_parser
