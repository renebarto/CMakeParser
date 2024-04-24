#include "cmake-parser/Variable.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class VariableTest
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

TEST_F(VariableTest, ConstructName)
{
    std::string name = "variable";
    Variable variable(name);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ("", variable.Value());
    EXPECT_EQ("Variable " + name + " = ", variable.Serialize());
}

TEST_F(VariableTest, ConstructNameValue)
{
    std::string name = "variable";
    std::string value = "value";
    Variable variable(name, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(value, variable.Value());
    EXPECT_EQ("Variable " + name + " = " + value, variable.Serialize());
}

TEST_F(VariableTest, SetValue)
{
    std::string name = "variable";
    std::string value = "value";
    std::string valueNew = "valueNew";
    Variable variable(name, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(value, variable.Value());
    variable.SetValue(valueNew);
    EXPECT_EQ(valueNew, variable.Value());
    EXPECT_EQ("Variable " + name + " = " + valueNew, variable.Serialize());
}

TEST_F(VariableTest, StreamInsertion)
{
    std::string name = "variable";
    std::string value = "value";
    Variable variable(name, value);

    EXPECT_EQ(name, variable.Name());
    EXPECT_EQ(value, variable.Value());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("Variable " + name + " = " + value, stream.str());
}

TEST_F(VariableTest, StreamInsertionPtr)
{
    std::string name = "variable";
    std::string value = "value";
    auto variable = std::make_shared<Variable>(name, value);

    EXPECT_EQ(name, variable->Name());
    EXPECT_EQ(value, variable->Value());
    std::ostringstream stream;
    stream << variable;
    EXPECT_EQ("Variable " + name + " = " + value, stream.str());
}

} // namespace cmake_parser
