#include "cmake-parser/VariableList.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class VariableListTest
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

TEST_F(VariableListTest, Construct)
{
    VariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("VariableList:\n", variables.Serialize());
}

TEST_F(VariableListTest, ConstructCopy)
{
    VariableList other;
    other.SetVariable("a", "x");
    other.SetVariable("b", "y");
    other.SetVariable("c", "z");

    VariableList variables(other);

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("z", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ("x", variables.GetVariable("a"));
    EXPECT_NOT_NULL(variables.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), variables.FindVariable("a"));
    EXPECT_EQ("y", variables.GetVariable("b"));
    EXPECT_NOT_NULL(variables.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), variables.FindVariable("b"));
    EXPECT_EQ("z", variables.GetVariable("c"));
    EXPECT_NOT_NULL(variables.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), variables.FindVariable("c"));
    EXPECT_EQ("VariableList:\nVariable a = x\nVariable b = y\nVariable c = z\n", variables.Serialize());
}

TEST_F(VariableListTest, Assign)
{
    VariableList other;
    other.SetVariable("a", "x");
    other.SetVariable("b", "y");
    other.SetVariable("c", "z");

    VariableList variables;
    
    variables = other;

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("z", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ("x", variables.GetVariable("a"));
    EXPECT_NOT_NULL(variables.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), variables.FindVariable("a"));
    EXPECT_EQ("y", variables.GetVariable("b"));
    EXPECT_NOT_NULL(variables.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), variables.FindVariable("b"));
    EXPECT_EQ("z", variables.GetVariable("c"));
    EXPECT_NOT_NULL(variables.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), variables.FindVariable("c"));
}

TEST_F(VariableListTest, SetVariable)
{
    VariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable("x", "y");

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    EXPECT_NOT_NULL(variables.FindVariable("x"));
    EXPECT_EQ("x", variables.FindVariable("x")->Name());
    EXPECT_EQ("y", variables.FindVariable("x")->Value());
}

TEST_F(VariableListTest, UnsetVariable)
{
    VariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable("x", "y");

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    EXPECT_NOT_NULL(variables.FindVariable("x"));

    variables.UnsetVariable("DUMMY");

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    EXPECT_NOT_NULL(variables.FindVariable("x"));

    variables.UnsetVariable("x");

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("", variables.GetVariable("x"));
    EXPECT_NULL(variables.FindVariable("x"));
}

TEST_F(VariableListTest, AddVariable)
{
    VariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.AddVariable("x", std::make_shared<Variable>("x", "y"));

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    EXPECT_NOT_NULL(variables.FindVariable("x"));
    EXPECT_EQ("x", variables.FindVariable("x")->Name());
    EXPECT_EQ("y", variables.FindVariable("x")->Value());
}

TEST_F(VariableListTest, SerializeJSONEmpty)
{
    VariableList variables;

    std::ostringstream stream;
    stream << variables.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[]", stream.str());
}

TEST_F(VariableListTest, SerializeJSON)
{
    VariableList variables;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "z" };
    variables.SetVariable(name1, value1);
    variables.SetVariable(name2, value2);
    variables.SetVariable(name3, value3);

    std::ostringstream stream;
    stream << variables.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[\n"
        "    {\n"
        "        \"name\": \"" + name1 + "\",\n"
        "        \"value\": \"" + value1 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name2 + "\",\n"
        "        \"value\": \"" + value2 + "\"\n"
        "    },\n"
        "    {\n"
        "        \"name\": \"" + name3 + "\",\n"
        "        \"value\": \"" + value3 + "\"\n"
        "    }\n"
        "]", stream.str());
}

TEST_F(VariableListTest, StreamInsertion)
{
    VariableList variables;
    std::string name1{ "a" };
    std::string name2{ "b" };
    std::string name3{ "c" };
    std::string value1{ "x" };
    std::string value2{ "y" };
    std::string value3{ "z" };
    variables.SetVariable(name1, value1);
    variables.SetVariable(name2, value2);
    variables.SetVariable(name3, value3);

    std::ostringstream stream;
    stream << variables;
    EXPECT_EQ(
        "VariableList:\n"
        "Variable " + name1 + " = " + value1 + "\n"
        "Variable " + name2 + " = " + value2 + "\n"
        "Variable " + name3 + " = " + value3 + "\n", stream.str());
}

} // namespace cmake_parser
