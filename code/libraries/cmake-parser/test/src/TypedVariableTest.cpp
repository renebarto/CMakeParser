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
    EXPECT_EQ("", variable.Description());
    EXPECT_EQ("TypedVariable " + name + ": =  ()", variable.Serialize());
}

TEST_F(TypedVariableTest, ConstructNameTypeValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string description = "\"description\"";
    TypedVariable variable(name, type, value, description);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ(description, variable.Description());
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value + " (" + description + ")", variable.Serialize());
}

TEST_F(TypedVariableTest, SetValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string valueNew = "valueNew";
    std::string description = "\"description\"";
    TypedVariable variable(name, type, value, description);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    variable.SetValue(valueNew);
    EXPECT_EQ(valueNew, variable.Value());
    EXPECT_EQ(description, variable.Description());
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + valueNew + " (" + description + ")", variable.Serialize());
}

TEST_F(TypedVariableTest, SetTypeValue)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string typeNew = "FILEPATH";
    std::string valueNew = "valueNew";
    std::string description = "\"description\"";
    TypedVariable variable(name, type, value, description);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ(description, variable.Description());
    variable.SetValue(typeNew, valueNew);
    EXPECT_EQ(typeNew, variable.Type());
    EXPECT_EQ(valueNew, variable.Value());
    EXPECT_EQ(description, variable.Description());
    EXPECT_EQ("TypedVariable " + name + ":" + typeNew + " = " + valueNew + " (" + description + ")", variable.Serialize());
}

TEST_F(TypedVariableTest, SetDescription)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string typeNew = "FILEPATH";
    std::string description = "\"description\"";
    std::string descriptionNew = "\"description new\"";
    TypedVariable variable(name, type, value, description);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ(description, variable.Description());
    variable.SetDescription(descriptionNew);
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ(descriptionNew, variable.Description());
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value +" (" + descriptionNew + ")", variable.Serialize());
}

TEST_F(TypedVariableTest, StreamInsertion)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string description = "\"description\"";
    TypedVariable variable(name, type, value, description);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(type, variable.Type());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ(description, variable.Description());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value + " (" + description + ")", stream.str());
}

TEST_F(TypedVariableTest, StreamInsertionPtr)
{
    std::string name = "variable";
    std::string type = "STRING";
    std::string value = "value";
    std::string description = "\"description\"";
    auto variable = std::make_shared<TypedVariable>(name, type, value, description);

    EXPECT_EQ(name, variable->Name());
    EXPECT_EQ(type, variable->Type());
    EXPECT_EQ(value, variable->Value());
    EXPECT_EQ(description, variable->Description());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("TypedVariable " + name + ":" + type + " = " + value + " (" + description + ")", stream.str());
}

} // namespace cmake_parser
