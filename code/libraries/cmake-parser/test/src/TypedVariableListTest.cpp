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
    other.SetVariable("a", "STRING", "x");
    other.SetVariable("b", "FILEPATH", "y");
    other.SetVariable("c", "BOOL", "ON");

    TypedVariableList variables(other);

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("ON", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ("x", variables.GetVariable("a"));
    EXPECT_NOT_NULL(variables.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), variables.FindVariable("a"));
    EXPECT_EQ("y", variables.GetVariable("b"));
    EXPECT_NOT_NULL(variables.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), variables.FindVariable("b"));
    EXPECT_EQ("ON", variables.GetVariable("c"));
    EXPECT_NOT_NULL(variables.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), variables.FindVariable("c"));
    EXPECT_EQ("TypedVariableList:\nTypedVariable a:STRING = x\nTypedVariable b:FILEPATH = y\nTypedVariable c:BOOL = ON\n", variables.Serialize());
}

TEST_F(TypedVariableListTest, Assign)
{
    TypedVariableList other;
    other.SetVariable("a", "STRING", "x");
    other.SetVariable("b", "FILEPATH", "y");
    other.SetVariable("c", "BOOL", "ON");

    TypedVariableList variables;
    
    variables = other;

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("ON", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ("x", variables.GetVariable("a"));
    EXPECT_NOT_NULL(variables.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), variables.FindVariable("a"));
    EXPECT_EQ("y", variables.GetVariable("b"));
    EXPECT_NOT_NULL(variables.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), variables.FindVariable("b"));
    EXPECT_EQ("ON", variables.GetVariable("c"));
    EXPECT_NOT_NULL(variables.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), variables.FindVariable("c"));
}

TEST_F(TypedVariableListTest, SetVariable)
{
    TypedVariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable("x", "STRING", "y");

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    ASSERT_NOT_NULL(variables.FindVariable("x"));
    EXPECT_EQ("x", variables.FindVariable("x")->Name());
    EXPECT_EQ("STRING", variables.FindVariable("x")->Type());
    EXPECT_EQ("y", variables.FindVariable("x")->Value());
}

TEST_F(TypedVariableListTest, UnsetTypedVariable)
{
    TypedVariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.SetVariable("x", "STRING", "y");

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

TEST_F(TypedVariableListTest, AddVariable)
{
    TypedVariableList variables;

    EXPECT_EQ(size_t{ 0 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));

    variables.AddVariable("x", std::make_shared<TypedVariable>("x", "STRING", "y"));

    EXPECT_EQ(size_t{ 1 }, variables.GetVariables().size());
    EXPECT_EQ("", variables.GetVariable("DUMMY"));
    EXPECT_NULL(variables.FindVariable("DUMMY"));
    EXPECT_EQ("y", variables.GetVariable("x"));
    EXPECT_NOT_NULL(variables.FindVariable("x"));
    EXPECT_EQ("x", variables.FindVariable("x")->Name());
    EXPECT_EQ("STRING", variables.FindVariable("x")->Type());
    EXPECT_EQ("y", variables.FindVariable("x")->Value());
}

TEST_F(TypedVariableListTest, StreamInsertion)
{
    TypedVariableList variables;
    variables.SetVariable("a", "STRING", "x");
    variables.SetVariable("b", "FILEPATH", "y");
    variables.SetVariable("c", "BOOL", "ON");

    EXPECT_EQ(size_t{ 3 }, variables.GetVariables().size());
    EXPECT_EQ("x", variables.GetVariable("a"));
    EXPECT_NOT_NULL(variables.FindVariable("a"));
    EXPECT_EQ("y", variables.GetVariable("b"));
    EXPECT_NOT_NULL(variables.FindVariable("b"));
    EXPECT_EQ("ON", variables.GetVariable("c"));
    EXPECT_NOT_NULL(variables.FindVariable("c"));

    std::ostringstream stream;
    stream << variables;
    EXPECT_EQ("TypedVariableList:\nTypedVariable a:STRING = x\nTypedVariable b:FILEPATH = y\nTypedVariable c:BOOL = ON\n", stream.str());
}

} // namespace cmake_parser
