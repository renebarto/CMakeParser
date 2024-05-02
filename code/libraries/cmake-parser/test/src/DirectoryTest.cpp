#include "cmake-parser/Directory.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class DirectoryTest
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

TEST_F(DirectoryTest, Construct)
{
    std::filesystem::path sourcePath{ "." };
    std::filesystem::path binaryPath{ "./cmake-bin" };
    Directory directory(sourcePath, binaryPath);

    EXPECT_EQ(sourcePath, directory.SourcePath());
    EXPECT_EQ(binaryPath, directory.BinaryPath());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(nullptr, directory.Parent());
    EXPECT_EQ("Directory sourcePath = \"" + sourcePath.string() + "\", binaryPath = \"" + binaryPath.string() + "\", parent = (none)", directory.Serialize());
}

TEST_F(DirectoryTest, ConstructWithParent)
{
    std::filesystem::path parentSourcePath{ "." };
    std::filesystem::path parentBinaryPath{ "./cmake-bin" };
    auto parent = std::make_shared< Directory>(parentSourcePath, parentBinaryPath, nullptr);
    std::filesystem::path sourcePath{ "subdir" };
    std::filesystem::path binaryPath{ "./cmake-bin/subdir" };
    Directory directory(sourcePath, binaryPath, parent);

    EXPECT_EQ(sourcePath, directory.SourcePath());
    EXPECT_EQ(binaryPath, directory.BinaryPath());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory sourcePath = \"" + sourcePath.string() + "\", binaryPath = \"" + binaryPath.string() + "\", parent = (Directory sourcePath = \"" + 
        parentSourcePath.string() + "\", binaryPath = \"" + parentBinaryPath.string() + "\")", directory.Serialize());
}

TEST_F(DirectoryTest, InheritVariablesFromParent)
{
    std::filesystem::path parentSourcePath{ "." };
    std::filesystem::path parentBinaryPath{ "./cmake-bin" };
    auto parent = std::make_shared< Directory>(parentSourcePath, parentBinaryPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path sourcePath{ "subdir" };
    std::filesystem::path binaryPath{ "./cmake-bin/subdir" };
    Directory directory(sourcePath, binaryPath, parent);

    EXPECT_EQ(sourcePath, directory.SourcePath());
    EXPECT_EQ(binaryPath, directory.BinaryPath());
    EXPECT_EQ(size_t{ 1 }, directory.GetVariables().size());
    auto var = directory.GetVariables().begin()->second;
    EXPECT_EQ("x", var->Name());
    EXPECT_EQ("y", var->Value());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory sourcePath = \"" + sourcePath.string() + "\", binaryPath = \"" + binaryPath.string() + "\", parent = (Directory sourcePath = \"" + 
        parentSourcePath.string() + "\", binaryPath = \"" + parentBinaryPath.string() + "\")", directory.Serialize());
}

TEST_F(DirectoryTest, SetVariable)
{
    std::filesystem::path parentSourcePath{ "." };
    std::filesystem::path parentBinaryPath{ "./cmake-bin" };
    auto parent = std::make_shared< Directory>(parentSourcePath, parentBinaryPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path sourcePath{ "subdir" };
    std::filesystem::path binaryPath{ "./cmake-bin/subdir" };
    Directory directory(sourcePath, binaryPath, parent);
    directory.SetVariable("a", "b");

    EXPECT_EQ(sourcePath, directory.SourcePath());
    EXPECT_EQ(binaryPath, directory.BinaryPath());
    EXPECT_EQ(size_t{ 2 }, directory.GetVariables().size());
    auto it = directory.GetVariables().begin();
    auto var = it->second;
    EXPECT_EQ("a", var->Name());
    EXPECT_EQ("b", var->Value());
    it++;
    var = it->second;
    EXPECT_EQ("x", var->Name());
    EXPECT_EQ("y", var->Value());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory sourcePath = \"" + sourcePath.string() + "\", binaryPath = \"" + binaryPath.string() + "\", parent = (Directory sourcePath = \"" + 
        parentSourcePath.string() + "\", binaryPath = \"" + parentBinaryPath.string() + "\")", directory.Serialize());
}

TEST_F(DirectoryTest, UnsetVariable)
{
    std::filesystem::path parentSourcePath{ "." };
    std::filesystem::path parentBinaryPath{ "./cmake-bin" };
    auto parent = std::make_shared< Directory>(parentSourcePath, parentBinaryPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path sourcePath{ "subdir" };
    std::filesystem::path binaryPath{ "./cmake-bin/subdir" };
    Directory directory(sourcePath, binaryPath, parent);
    directory.UnsetVariable("x");

    EXPECT_EQ(sourcePath, directory.SourcePath());
    EXPECT_EQ(binaryPath, directory.BinaryPath());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory sourcePath = \"" + sourcePath.string() + "\", binaryPath = \"" + binaryPath.string() + "\", parent = (Directory sourcePath = \"" + 
        parentSourcePath.string() + "\", binaryPath = \"" + parentBinaryPath.string() + "\")", directory.Serialize());
}

} // namespace cmake_parser
