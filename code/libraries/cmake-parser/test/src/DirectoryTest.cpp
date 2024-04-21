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
    std::filesystem::path path{ "." };
    Directory directory(path);

    EXPECT_EQ(path, directory.Path());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(nullptr, directory.Parent());
    EXPECT_EQ("Directory path = \"" + path.string() + "\", parent = \"none\"", directory.Serialize());
}

TEST_F(DirectoryTest, ConstructWithParent)
{
    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared< Directory>(parentPath, nullptr);
    std::filesystem::path path{ "subdir" };
    Directory directory(path, parent);

    EXPECT_EQ(path, directory.Path());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"", directory.Serialize());
}

TEST_F(DirectoryTest, InheritVariablesFromParent)
{
    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared< Directory>(parentPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path path{ "subdir" };
    Directory directory(path, parent);

    EXPECT_EQ(path, directory.Path());
    EXPECT_EQ(size_t{ 1 }, directory.GetVariables().size());
    auto var = directory.GetVariables().begin()->second;
    EXPECT_EQ("x", var->Name());
    EXPECT_EQ("y", var->Value());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"", directory.Serialize());
}

TEST_F(DirectoryTest, SetVariable)
{
    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared< Directory>(parentPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path path{ "subdir" };
    Directory directory(path, parent);
    directory.SetVariable("a", "b");

    EXPECT_EQ(path, directory.Path());
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
    EXPECT_EQ("Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"", directory.Serialize());
}

TEST_F(DirectoryTest, UnsetVariable)
{
    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared< Directory>(parentPath, nullptr);
    parent->SetVariable("x", "y");
    std::filesystem::path path{ "subdir" };
    Directory directory(path, parent);
    directory.UnsetVariable("x");

    EXPECT_EQ(path, directory.Path());
    EXPECT_EQ(size_t{ 0 }, directory.GetVariables().size());
    EXPECT_EQ(parent, directory.Parent());
    EXPECT_EQ("Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"", directory.Serialize());
}

} // namespace cmake_parser
