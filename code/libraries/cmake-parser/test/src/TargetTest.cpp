#include "cmake-parser/Target.h"

#include "test-platform/GoogleTest.h"
#include "serialization/EnumSerialization.h"

namespace cmake_parser {

class TargetTest
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

TEST_F(TargetTest, ConstructName)
{
    std::string name = "target";
    Target target(name);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(TargetAttribute::None, target.Attributes());
    EXPECT_EQ("", target.Sources());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + " [] Attributes None", target.Serialize());
}

TEST_F(TargetTest, ConstructNameAttributes)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    Target target(name, attributes);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ("", target.Sources());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + " [] Attributes Win32Exe", target.Serialize());
}

TEST_F(TargetTest, ConstructNameAttributesSources)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    Target target(name, attributes, sources);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ(sources, target.Sources());
    EXPECT_EQ("", target.AliasTarget());
    EXPECT_EQ("Target " + name + " [" + sources + "] Attributes Win32Exe", target.Serialize());
}

TEST_F(TargetTest, ConstructNameAttributesAlias)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources;
    std::string aliasTarget{ "othertarget" };
    Target target(name, attributes, sources, aliasTarget);

    EXPECT_EQ(name, target.Name());
    EXPECT_EQ(attributes, target.Attributes());
    EXPECT_EQ(sources, target.Sources());
    EXPECT_EQ(aliasTarget, target.AliasTarget());
    EXPECT_EQ("Target " + name + " [" + sources + "] Attributes Alias " + aliasTarget, target.Serialize());
}

TEST_F(TargetTest, SerializeJSONEmpty)
{
    std::string name = "target";
    Target target(name);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"attributes\": \"None\",\n"
        "    \"sources\": null\n"
        "}", stream.str());
}

TEST_F(TargetTest, SerializeJSON)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    Target target(name, attributes, sources);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"attributes\": \"" + serialization::Serialize(target.Attributes()) + "\",\n"
        "    \"sources\": \"" + target.Sources() + "\"\n"
        "}", stream.str());
}

TEST_F(TargetTest, SerializeJSONAlias)
{
    std::string name = "alias";
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources{ "" };
    std::string aliasTarget{ "target" };
    Target target(name, attributes, sources, aliasTarget);

    std::ostringstream stream;
    stream << target.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + name + "\",\n"
        "    \"attributes\": \"" + serialization::Serialize(target.Attributes()) + "\",\n"
        "    \"sources\": null,\n"
        "    \"aliasTarget\": \"" + target.AliasTarget() + "\"\n" +
        "}", stream.str());
}

TEST_F(TargetTest, StreamInsertion)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    Target target(name, attributes, sources);

    EXPECT_EQ(name, target.Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + " [" + sources + "] Attributes " + serialization::Serialize(target.Attributes()), stream.str());
}

TEST_F(TargetTest, StreamInsertionAlias)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Alias };
    std::string sources{ "" };
    std::string aliasTarget{ "target" };
    Target target(name, attributes, sources, aliasTarget);

    EXPECT_EQ(name, target.Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + " [] Attributes " + serialization::Serialize(target.Attributes()) + " " + aliasTarget, stream.str());
}

TEST_F(TargetTest, StreamInsertionPtr)
{
    std::string name = "target";
    TargetAttribute attributes{ TargetAttribute::Win32Exe };
    std::string sources{ "main.cpp" };
    auto target = std::make_shared<Target>(name, attributes, sources);

    EXPECT_EQ(name, target->Name());
    std::ostringstream stream;
    stream << target;
    EXPECT_EQ("Target " + name + " [" + sources + "] Attributes Win32Exe", stream.str());
}

} // namespace cmake_parser
