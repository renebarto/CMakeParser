#include "cmake-parser/TypedVariableList.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class TypedVariableListTest
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

TEST_F(TypedVariableListTest, Construct)
{
    TypedVariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("TypedVariableList:\n", variables.Serialize());
}

TEST_F(TypedVariableListTest, ConstructCopy)
{
    TypedVariableList other;
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

    TypedVariableList variables(other);

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ(value1, other.GetVariable(name1));
    EXPECT_NOT_NULL(other.FindVariable(name1));
    EXPECT_EQ(value2, other.GetVariable(name2));
    EXPECT_NOT_NULL(other.FindVariable(name2));
    EXPECT_EQ(value3, other.GetVariable(name3));
    EXPECT_NOT_NULL(other.FindVariable(name3));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ(value1, variables.GetVariable(name1));
    EXPECT_NOT_NULL(variables.FindVariable(name1));
    EXPECT_NE(other.FindVariable(name1), variables.FindVariable(name1));
    EXPECT_EQ(value2, variables.GetVariable(name2));
    EXPECT_NOT_NULL(variables.FindVariable(name2));
    EXPECT_NE(other.FindVariable(name2), variables.FindVariable(name2));
    EXPECT_EQ(value3, variables.GetVariable(name3));
    EXPECT_NOT_NULL(variables.FindVariable(name3));
    EXPECT_NE(other.FindVariable(name3), variables.FindVariable(name3));
    EXPECT_EQ(
        "TypedVariableList:\n"
        "TypedVariable " + name1 + ":" + type1 + " = " + value1 + " (" + description1 + ")\n"
        "TypedVariable " + name2 + ":" + type2 + " = " + value2 + " (" + description2 + ")\n"
        "TypedVariable " + name3 + ":" + type3 + " = " + value3 + " (" + description3 + ")\n", variables.Serialize());
}

TEST_F(TypedVariableListTest, Assign)
{
    TypedVariableList other;
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

    TypedVariableList variables;
    
    variables = other;

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ(value1, other.GetVariable(name1));
    EXPECT_NOT_NULL(other.FindVariable(name1));
    EXPECT_EQ(value2, other.GetVariable(name2));
    EXPECT_NOT_NULL(other.FindVariable(name2));
    EXPECT_EQ(value3, other.GetVariable(name3));
    EXPECT_NOT_NULL(other.FindVariable(name3));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ(value1, variables.GetVariable(name1));
    EXPECT_NOT_NULL(variables.FindVariable(name1));
    EXPECT_NE(other.FindVariable(name1), variables.FindVariable(name1));
    EXPECT_EQ(value2, variables.GetVariable(name2));
    EXPECT_NOT_NULL(variables.FindVariable(name2));
    EXPECT_NE(other.FindVariable(name2), variables.FindVariable(name2));
    EXPECT_EQ(value3, variables.GetVariable(name3));
    EXPECT_NOT_NULL(variables.FindVariable(name3));
    EXPECT_NE(other.FindVariable(name3), variables.FindVariable(name3));
}

TEST_F(TypedVariableListTest, SetVariable)
{
    TypedVariableList variables;

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable(name, type, value, description);

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ(value, variables.GetVariable(name));
    ASSERT_NOT_NULL(variables.FindVariable(name));
    EXPECT_EQ(name, variables.FindVariable(name)->Name());
    EXPECT_EQ(type, variables.FindVariable(name)->Type());
    EXPECT_EQ(value, variables.FindVariable(name)->Value());
    EXPECT_EQ(description, variables.FindVariable(name)->Description());
}

TEST_F(TypedVariableListTest, UnsetTypedVariable)
{
    TypedVariableList variables;

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable(name, type, value, description);

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ(value, variables.GetVariable(name));
    EXPECT_NOT_NULL(variables.FindVariable(name));

    variables.UnsetVariable("DUMMY");

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ(value, variables.GetVariable(name));
    EXPECT_NOT_NULL(variables.FindVariable(name));

    variables.UnsetVariable(name);

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("", variables.GetVariable(name));
    EXPECT_NULL(variables.FindVariable(name));
}

TEST_F(TypedVariableListTest, AddVariable)
{
    TypedVariableList variables;

    std::string name{ "x" };
    std::string type{ "STRING" };
    std::string value{ "y" };
    std::string description{ "var x" };
    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.AddVariable(name, std::make_shared<TypedVariable>(name, type, value, description));

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable(name));
    EXPECT_NOT_NULL(variables.FindVariable(name));
    EXPECT_EQ(name, variables.FindVariable(name)->Name());
    EXPECT_EQ(type, variables.FindVariable(name)->Type());
    EXPECT_EQ("y", variables.FindVariable(name)->Value());
    EXPECT_EQ(description, variables.FindVariable(name)->Description());
}

TEST_F(TypedVariableListTest, SerializeJSONEmpty)
{
    TypedVariableList variables;

    std::ostringstream stream;
    stream << variables.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ("[]", stream.str());
}

TEST_F(TypedVariableListTest, SerializeJSON)
{
    TypedVariableList variables;
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
    variables.SetVariable(name1, type1, value1, description1);
    variables.SetVariable(name2, type2, value2, description2);
    variables.SetVariable(name3, type3, value3, description3);

    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ(value1, variables.GetVariable(name1));
    EXPECT_NOT_NULL(variables.FindVariable(name1));
    EXPECT_EQ(value2, variables.GetVariable(name2));
    EXPECT_NOT_NULL(variables.FindVariable(name2));
    EXPECT_EQ(value3, variables.GetVariable(name3));
    EXPECT_NOT_NULL(variables.FindVariable(name3));

    std::ostringstream stream;
    stream << variables.Serialize(SerializationFormat::JSON, 0);
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

TEST_F(TypedVariableListTest, StreamInsertion)
{
    TypedVariableList variables;
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
    variables.SetVariable(name1, type1, value1, description1);
    variables.SetVariable(name2, type2, value2, description2);
    variables.SetVariable(name3, type3, value3, description3);

    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ(value1, variables.GetVariable(name1));
    EXPECT_NOT_NULL(variables.FindVariable(name1));
    EXPECT_EQ(value2, variables.GetVariable(name2));
    EXPECT_NOT_NULL(variables.FindVariable(name2));
    EXPECT_EQ(value3, variables.GetVariable(name3));
    EXPECT_NOT_NULL(variables.FindVariable(name3));

    std::ostringstream stream;
    stream << variables;
    EXPECT_EQ(
        "TypedVariableList:\n"
        "TypedVariable " + name1 + ":" + type1 + " = " + value1 + " (" + description1 + ")\n"
        "TypedVariable " + name2 + ":" + type2 + " = " + value2 + " (" + description2 + ")\n"
        "TypedVariable " + name3 + ":" + type3 + " = " + value3 + " (" + description3 + ")\n", stream.str());
}

} // namespace cmake_parser
