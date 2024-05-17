#include "cmake-parser/Target.h"

#include "test-platform/GoogleTest.h"
#include "serialization/EnumSerialization.h"
#include "cmake-parser/Directory.h"
#include "cmake-parser/Project.h"

namespace cmake_parser {

class TargetTest
    : public ::testing::Test
{
public:
    DirectoryPtr directory;
    ProjectPtr project;

    TargetTest()
        : directory{ std::make_shared<Directory>("", "") }
        , project{ std::make_shared<Project>(directory) }
    {
    }
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(TargetTest, ConstructName)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    Target target(project, name, type);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(type, target.Type());
    EXPECT_EQ(TargetAttribute::None, target.Attributes());
    EXPECT_EQ("", target.Sources().ToString());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [] Attributes None", target.Serialize());
    EXPECT_FALSE(target.IsGlobal());
    EXPECT_FALSE(target.IsAlias());
    EXPECT_FALSE(target.IsImported());
}

TEST_F(TargetTest, ConstructNameAttributes)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    Target target(project, name, type, attributes);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(type, target.Type());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ("", target.Sources().ToString());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [] Attributes Win32Exe", target.Serialize());
}

TEST_F(TargetTest, ConstructNameAttributesSources)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Global };
    std::string sources{ "main.cpp" };
    Target target(project, name, type, attributes, sources);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(type, target.Type());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ(sources, target.Sources().ToString());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [" + sources + "] Attributes Global", target.Serialize());
    EXPECT_TRUE(target.IsGlobal());
    EXPECT_FALSE(target.IsAlias());
    EXPECT_FALSE(target.IsImported());
}

TEST_F(TargetTest, ConstructNameAttributesAlias)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources;
    std::string aliasTarget{ "othertarget" };
    Target target(project, name, type, attributes, sources, aliasTarget);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(type, target.Type());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ(sources, target.Sources().ToString());
    EXPECT_EQ(aliasTarget, target.AliasTarget());
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [" + sources + "] Attributes Alias " + aliasTarget, target.Serialize());
    EXPECT_FALSE(target.IsGlobal());
    EXPECT_TRUE(target.IsAlias());
    EXPECT_FALSE(target.IsImported());
}

TEST_F(TargetTest, ConstructNameAttributesImported)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Imported };
    std::string sources;
    Target target(project, name, type, attributes, sources);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(type, target.Type());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ(sources, target.Sources().ToString());
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [" + sources + "] Attributes Imported", target.Serialize());
    EXPECT_FALSE(target.IsGlobal());
    EXPECT_FALSE(target.IsAlias());
    EXPECT_TRUE(target.IsImported());
}

TEST_F(TargetTest, SerializeJSONEmpty)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    Target target(project, name, type);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"type\": \"Executable\",\n"
        "    \"attributes\": \"None\",\n"
        "    \"sources\": \n"
        "        [],\n"
        "    \"properties\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(TargetTest, SerializeJSON)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    Target target(project, name, type, attributes, sources);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"type\": \"Executable\",\n"
        "    \"attributes\": \"" + serialization::Serialize(target.Attributes()) + "\",\n"
        "    \"sources\": \n" + target.Sources().Serialize(SerializationFormat::JSON, 8) + ",\n"
        "    \"properties\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(TargetTest, SerializeJSONAlias)
{
    std::string name = "alias";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources{ "" };
    std::string aliasTarget{ "target" };
    Target target(project, name, type, attributes, sources, aliasTarget);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"type\": \"Executable\",\n"
        "    \"attributes\": \"" + serialization::Serialize(target.Attributes()) + "\",\n"
        "    \"sources\": \n"
        "        [],\n"
        "    \"properties\": \n"
        "        [],\n"
        "    \"aliasTarget\": \"" + target.AliasTarget() + "\"\n" +
        "}", stream.str());
}

TEST_F(TargetTest, StreamInsertion)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    Target target(project, name, type, attributes, sources);

    EXPECT_EQ(name, target.Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [" + sources + "] Attributes " + serialization::Serialize(target.Attributes()), stream.str());
}

TEST_F(TargetTest, StreamInsertionAlias)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources{ "" };
    std::string aliasTarget{ "target" };
    Target target(project, name, type, attributes, sources, aliasTarget);

    EXPECT_EQ(name, target.Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [] Attributes " + serialization::Serialize(target.Attributes()) + " " + aliasTarget, stream.str());
}

TEST_F(TargetTest, StreamInsertionPtr)
{
    std::string name = "target";
    TargetType type{ TargetType::Executable };
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    auto target = std::make_shared<Target>(project, name, type, attributes, sources);

    EXPECT_EQ(name, target->Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + ":" + serialization::Serialize(type) + " [" + sources + "] Attributes Win32Exe", stream.str());
}

} // namespace cmake_parser
