#include "cmake-parser/CMakeCache.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class CMakeCacheTest
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

TEST_F(CMakeCacheTest, Construct)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("CMakeCache:\n", cache.Serialize());
}

TEST_F(CMakeCacheTest, ConstructCopy)
{
    CMakeCache other;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string type1{ "STRING" };
    std::string type2{ "FILEPATH" };
    std::string type3{ "BOOL" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "ON" };
    std::string description1{ "var a" };
    std::string description2{ "var b" };
    std::string description3{ "var c" };
    other.SetVariable(name1, type1, value1, description1);
    other.SetVariable(name2, type2, value2, description2);
    other.SetVariable(name3, type3, value3, description3);

    CMakeCache cache(other);

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ(value1, other.GetVariable(name1));
    EXPECT_NOT_NULL(other.FindVariable(name1));
    EXPECT_EQ(value2, other.GetVariable(name2));
    EXPECT_NOT_NULL(other.FindVariable(name2));
    EXPECT_EQ(value3, other.GetVariable(name3));
    EXPECT_NOT_NULL(other.FindVariable(name3));
    EXPECT_EQ(size_t{ 3 }, cache.GetVariables().size());
    EXPECT_EQ(value1, cache.GetVariable(name1));
    EXPECT_NOT_NULL(cache.FindVariable(name1));
    EXPECT_NE(other.FindVariable(name1), cache.FindVariable(name1));
    EXPECT_EQ(value2, cache.GetVariable(name2));
    EXPECT_NOT_NULL(cache.FindVariable(name2));
    EXPECT_NE(other.FindVariable(name2), cache.FindVariable(name2));
    EXPECT_EQ(value3, cache.GetVariable(name3));
    EXPECT_NOT_NULL(cache.FindVariable(name3));
    EXPECT_NE(other.FindVariable(name3), cache.FindVariable(name3));
    EXPECT_EQ(
        "CMakeCache:\n"
        "TypedVariable " + name1 + ":" + type1 + " = " + value1 + " (" + description1 + ")\n"
        "TypedVariable " + name2 + ":" + type2 + " = " + value2 + " (" + description2 + ")\n"
        "TypedVariable " + name3 + ":" + type3 + " = " + value3 + " (" + description3 + ")\n", cache.Serialize());
}

TEST_F(CMakeCacheTest, Assign)
{
    CMakeCache other;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string type1{ "STRING" };
    std::string type2{ "FILEPATH" };
    std::string type3{ "BOOL" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "ON" };
    std::string description1{ "var a" };
    std::string description2{ "var b" };
    std::string description3{ "var c" };
    other.SetVariable(name1, type1, value1, description1);
    other.SetVariable(name2, type2, value2, description2);
    other.SetVariable(name3, type3, value3, description3);

    CMakeCache cache;
    
    cache = other;

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ(value1, other.GetVariable(name1));
    EXPECT_NOT_NULL(other.FindVariable(name1));
    EXPECT_EQ(value2, other.GetVariable(name2));
    EXPECT_NOT_NULL(other.FindVariable(name2));
    EXPECT_EQ(value3, other.GetVariable(name3));
    EXPECT_NOT_NULL(other.FindVariable(name3));
    EXPECT_EQ(size_t{ 3 }, cache.GetVariables().size());
    EXPECT_EQ(value1, cache.GetVariable(name1));
    EXPECT_NOT_NULL(cache.FindVariable(name1));
    EXPECT_NE(other.FindVariable(name1), cache.FindVariable(name1));
    EXPECT_EQ(value2, cache.GetVariable(name2));
    EXPECT_NOT_NULL(cache.FindVariable(name2));
    EXPECT_NE(other.FindVariable(name2), cache.FindVariable(name2));
    EXPECT_EQ(value3, cache.GetVariable(name3));
    EXPECT_NOT_NULL(cache.FindVariable(name3));
    EXPECT_NE(other.FindVariable(name3), cache.FindVariable(name3));
}

TEST_F(CMakeCacheTest, SetVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    cache.SetVariable(name, type, value, description);

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ(value, cache.GetVariable(name));
    EXPECT_NOT_NULL(cache.FindVariable(name));
    EXPECT_EQ(name, cache.FindVariable(name)->Name());
    EXPECT_EQ(type, cache.FindVariable(name)->Type());
    EXPECT_EQ(value, cache.FindVariable(name)->Value());
    EXPECT_EQ(description, cache.FindVariable(name)->Description());
}

TEST_F(CMakeCacheTest, UnsetVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    cache.SetVariable(name, type, value, description);

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ(value, cache.GetVariable(name));
    EXPECT_NOT_NULL(cache.FindVariable(name));

    cache.UnsetVariable("DUMMY");

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ(value, cache.GetVariable(name));
    EXPECT_NOT_NULL(cache.FindVariable(name));

    cache.UnsetVariable(name);

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("", cache.GetVariable(name));
    EXPECT_NULL(cache.FindVariable(name));
}

TEST_F(CMakeCacheTest, AddVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    cache.AddVariable(name, std::make_shared<TypedVariable>(name, type, value, description));

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ(value, cache.GetVariable(name));
    EXPECT_NOT_NULL(cache.FindVariable(name));
    EXPECT_EQ(name, cache.FindVariable(name)->Name());
    EXPECT_EQ(type, cache.FindVariable(name)->Type());
    EXPECT_EQ(value, cache.FindVariable(name)->Value());
    EXPECT_EQ(description, cache.FindVariable(name)->Description());
}

TEST_F(CMakeCacheTest, SerializeJSONEmpty)
{
    CMakeCache cache;

    std::ostringstream stream;
    stream << cache.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ("[]", stream.str());
}

TEST_F(CMakeCacheTest, SerializeJSON)
{
    CMakeCache cache;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string type1{ "STRING" };
    std::string type2{ "FILEPATH" };
    std::string type3{ "BOOL" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "ON" };
    std::string description1{ "var a" };
    std::string description2{ "var b" };
    std::string description3{ "var c" };
    cache.SetVariable(name1, type1, value1, description1);
    cache.SetVariable(name2, type2, value2, description2);
    cache.SetVariable(name3, type3, value3, description3);

    std::ostringstream stream;
    stream << cache.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[\n"
        "    {\n"
        "        \"name\": \"" + name1 + "\",\n"
        "        \"type\": \"" + type1 + "\",\n"
        "        \"value\": \"" + value1 + "\",\n"
        "        \"description\": \"" + description1 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name2 + "\",\n"
        "        \"type\": \"" + type2 + "\",\n"
        "        \"value\": \"" + value2 + "\",\n"
        "        \"description\": \"" + description2 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name3 + "\",\n"
        "        \"type\": \"" + type3 + "\",\n"
        "        \"value\": \"" + value3 + "\",\n"
        "        \"description\": \"" + description3 + "\"\n"
        "    }\n"
        "]", stream.str());
}

TEST_F(CMakeCacheTest, StreamInsertion)
{
    CMakeCache cache;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string type1{ "STRING" };
    std::string type2{ "FILEPATH" };
    std::string type3{ "BOOL" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "ON" };
    std::string description1{ "var a" };
    std::string description2{ "var b" };
    std::string description3{ "var c" };
    cache.SetVariable(name1, type1, value1, description1);
    cache.SetVariable(name2, type2, value2, description2);
    cache.SetVariable(name3, type3, value3, description3);

    std::ostringstream stream;
    stream << cache;
    EXPECT_EQ(
        "CMakeCache:\n"
        "TypedVariable " + name1 + ":" + type1 + " = " + value1 + " (" + description1 + ")\n"
        "TypedVariable " + name2 + ":" + type2 + " = " + value2 + " (" + description2 + ")\n"
        "TypedVariable " + name3 + ":" + type3 + " = " + value3 + " (" + description3 + ")\n", stream.str());
}

} // namespace cmake_parser
