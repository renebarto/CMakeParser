#include "cmake-parser/TypedVariable.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class TypedVariableTest
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

TEST_F(TypedVariableTest, ConstructName)
{
    std::string name = "variable";
    TypedVariable variable(name);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ("", variable.Type());
    EXPECT_EQ("", variable.Value());
    EXPECT_EQ("TypedVariable " + name + ": = ", variable.Serialize());
}

TEST_F(TypedVariableTest, ConstructNameTypeValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    TypedVariable variable(name, type, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value, variable.Serialize());
}

TEST_F(TypedVariableTest, SetValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string valueNew = "valueNew";
    TypedVariable variable(name, type, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    variable.SetValue(valueNew);
    EXPECT_EQ(valueNew, variable.Value());
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + valueNew, variable.Serialize());
}

TEST_F(TypedVariableTest, SetTypeValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string typeNew = "FILEPATH";
    std::string valueNew = "valueNew";
    TypedVariable variable(name, type, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    variable.SetValue(typeNew, valueNew);
    EXPECT_EQ(typeNew, variable.Type());
    EXPECT_EQ(valueNew, variable.Value());
    EXPECT_EQ("TypedVariable " + name + ":" + typeNew + " = " + valueNew, variable.Serialize());
}

TEST_F(TypedVariableTest, StreamInsertion)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    TypedVariable variable(name, type, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value, stream.str());
}

TEST_F(TypedVariableTest, StreamInsertionPtr)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    auto variable = std::make_shared<TypedVariable>(name, type, value);

    EXPECT_EQ(name, variable->Name());
    EXPECT_EQ(type, variable->Type());
    EXPECT_EQ(value, variable->Value());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value, stream.str());
}

} // namespace cmake_parser
