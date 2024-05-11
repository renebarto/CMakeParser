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
    EXPECT_EQ("Directory sourcePath = " + sourcePath.string() + ", binaryPath = " + binaryPath.string() + ", parent = (none)", directory.Serialize());
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
    EXPECT_EQ("Directory sourcePath = " + sourcePath.string() + ", binaryPath = " + binaryPath.string() + ", parent = (Directory sourcePath = " + 
        parentSourcePath.string() + ", binaryPath = " + parentBinaryPath.string() + ")", directory.Serialize());
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
    EXPECT_EQ("Directory sourcePath = " + sourcePath.string() + ", binaryPath = " + binaryPath.string() + ", parent = (Directory sourcePath = " + 
        parentSourcePath.string() + ", binaryPath = " + parentBinaryPath.string() + ")", directory.Serialize());
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
    EXPECT_EQ("Directory sourcePath = " + sourcePath.string() + ", binaryPath = " + binaryPath.string() + ", parent = (Directory sourcePath = " + 
        parentSourcePath.string() + ", binaryPath = " + parentBinaryPath.string() + ")", directory.Serialize());
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
    EXPECT_EQ("Directory sourcePath = " + sourcePath.string() + ", binaryPath = " + binaryPath.string() + ", parent = (Directory sourcePath = " + 
        parentSourcePath.string() + ", binaryPath = " + parentBinaryPath.string() + ")", directory.Serialize());
}

TEST_F(DirectoryTest, SerializeJSONEmpty)
{
    std::string sourcePath = "dir/src";
    std::string binaryPath = "dir/bin";
    Directory directory(sourcePath, binaryPath);

    std::ostringstream stream;
    stream << directory.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"sourcePath\": \"" + directory.SourcePath().generic_string() + "\",\n"
        "    \"binaryPath\": \"" + directory.BinaryPath().generic_string() + "\",\n"
        "    \"parent\": null\n"
        "}", stream.str());
}

TEST_F(DirectoryTest, SerializeJSON)
{
    std::string parentSourcePath = "parent/src";
    std::string parentBinaryPath = "parent/bin";
    auto parent = std::make_shared<Directory>(parentSourcePath, parentBinaryPath);
    std::string sourcePath = "dir/src";
    std::string binaryPath = "dir/bin";
    Directory directory(sourcePath, binaryPath, parent);

    std::ostringstream stream;
    stream << parent->Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"sourcePath\": \"" + parent->SourcePath().generic_string() + "\",\n"
        "    \"binaryPath\": \"" + parent->BinaryPath().generic_string() + "\",\n"
        "    \"parent\": null\n"
        "}", stream.str());
    stream.str("");
    stream << directory.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"sourcePath\": \"" + directory.SourcePath().generic_string() + "\",\n"
        "    \"binaryPath\": \"" + directory.BinaryPath().generic_string() + "\",\n"
        "    \"parent\": \"" + parent->SourcePath().generic_string() + "\"\n"
        "}", stream.str());
}

TEST_F(DirectoryTest, StreamInsertion)
{
    std::string parentSourcePath = "parent/src";
    std::string parentBinaryPath = "parent/bin";
    auto parent = std::make_shared<Directory>(parentSourcePath, parentBinaryPath);
    std::string sourcePath = "dir/src";
    std::string binaryPath = "dir/bin";
    Directory directory(sourcePath, binaryPath, parent);

    std::ostringstream stream;
    stream << *parent;
    EXPECT_EQ("Directory sourcePath = " + parentSourcePath + ", binaryPath = " + parentBinaryPath + ", parent = (none)", stream.str());
    stream.str("");
    stream << directory;
    EXPECT_EQ("Directory sourcePath = " + sourcePath + ", binaryPath = " + binaryPath + ", parent = (" + parent->SerializeShort() + ")", stream.str());
}

TEST_F(DirectoryTest, StreamInsertionPtr)
{
    std::string parentSourcePath = "parent/src";
    std::string parentBinaryPath = "parent/bin";
    auto parent = std::make_shared<Directory>(parentSourcePath, parentBinaryPath);
    std::string sourcePath = "dir/src";
    std::string binaryPath = "dir/bin";
    auto directory = std::make_shared<Directory>(sourcePath, binaryPath, parent);

    std::ostringstream stream;
    stream << parent;
    EXPECT_EQ("Directory sourcePath = " + parentSourcePath + ", binaryPath = " + parentBinaryPath + ", parent = (none)", stream.str());
    stream.str("");
    stream << directory;
    EXPECT_EQ("Directory sourcePath = " + sourcePath + ", binaryPath = " + binaryPath + ", parent = (" + parent->SerializeShort() + ")", stream.str());
}

} // namespace cmake_parser
