#include "cmake-parser/TargetList.h"

#include "test-platform/GoogleTest.h"
#include "cmake-parser/Directory.h"
#include "cmake-parser/Project.h"

namespace cmake_parser {

class TargetListTest
    : public ::testing::Test
{
public:
    DirectoryPtr directory;
    ProjectPtr project;

    TargetListTest()
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

TEST_F(TargetListTest, Construct)
{
    TargetList targets;

    EXPECT_EQ(size_t{ 0 }, targets.GetTargets().size());
    EXPECT_NULL(targets.FindTarget("DUMMY"));
    EXPECT_EQ("TargetList:\n", targets.Serialize());
}

TEST_F(TargetListTest, SerializeJSONEmpty)
{
    TargetList targets;

    std::ostringstream stream;
    stream << targets.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[]", stream.str());
}

TEST_F(TargetListTest, SerializeJSON)
{
    TargetList targets;
    std::string name1{ "target1" };
    std::string name2{ "target2" };
    std::string name3{ "target3" };
    std::string sources1{ "main1.cpp" };
    std::string sources2{ "main2.cpp" };
    std::string sources3{ "" };
    std::string aliasTarget3{ "aliasTarget" };
    auto target1 = std::make_shared<Target>(project, name1, TargetType::Executable, TargetAttribute::None, sources1);
    auto target2 = std::make_shared<Target>(project, name2, TargetType::StaticLibrary, TargetAttribute::None, sources2);
    auto target3 = std::make_shared<Target>(project, name3, TargetType::Object, TargetAttribute::Alias, sources3, aliasTarget3);
    targets.AddTarget(name1, target1);
    targets.AddTarget(name2, target2);
    targets.AddTarget(name3, target3);

    std::ostringstream stream;
    stream << targets.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[\n"
        "    {\n"
        "        \"name\": \"" + name1 + "\",\n"
        "        \"type\": \"Executable\",\n"
        "        \"attributes\": \"None\",\n"
        "        \"sources\": \"" + sources1 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name2 + "\",\n"
        "        \"type\": \"StaticLibrary\",\n"
        "        \"attributes\": \"None\",\n"
        "        \"sources\": \"" + sources2 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name3 + "\",\n"
        "        \"type\": \"Object\",\n"
        "        \"attributes\": \"Alias\",\n"
        "        \"sources\": null,\n"
        "        \"aliasTarget\": \"" + aliasTarget3 + "\"\n"
        "    }\n"
        "]", stream.str());
}

TEST_F(TargetListTest, StreamInsertion)
{
    TargetList targets;
    std::string name1{ "target1" };
    std::string name2{ "target2" };
    std::string name3{ "target3" };
    std::string sources1{ "main1.cpp" };
    std::string sources2{ "main2.cpp" };
    std::string sources3{ "main3.cpp" };
    auto target1 = std::make_shared<Target>(project, name1, TargetType::Executable, TargetAttribute::None, sources1);
    auto target2 = std::make_shared<Target>(project, name2, TargetType::StaticLibrary, TargetAttribute::None, sources2);
    auto target3 = std::make_shared<Target>(project, name3, TargetType::Object, TargetAttribute::None, sources3);
    targets.AddTarget(name1, target1);
    targets.AddTarget(name2, target2);
    targets.AddTarget(name3, target3);

    EXPECT_EQ(size_t{ 3 }, targets.GetTargets().size());
    EXPECT_NOT_NULL(targets.FindTarget(name1));
    EXPECT_NOT_NULL(targets.FindTarget(name2));
    EXPECT_NOT_NULL(targets.FindTarget(name3));

    std::ostringstream stream;
    stream << targets;
    EXPECT_EQ(
        "TargetList:\n"
        "Target target1:Executable [main1.cpp] Attributes None\n"
        "Target target2:StaticLibrary [main2.cpp] Attributes None\n"
        "Target target3:Object [main3.cpp] Attributes None\n", stream.str());
}

} // namespace cmake_parser
