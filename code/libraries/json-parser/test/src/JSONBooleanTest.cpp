#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/JSONBoolean.h"

namespace json_parser {

class JSONBooleanTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONBooleanTest, Construct)
{
    JSONBoolean target;

    EXPECT_EQ(JSONValueType::Boolean, target.Type());
    EXPECT_FALSE(target.GetValue());
}

TEST_F(JSONBooleanTest, ConstructInitializer)
{
    bool value = true;
    JSONBoolean target(value);

    EXPECT_EQ(JSONValueType::Boolean, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_F(JSONBooleanTest, SerializeFalse)
{
    JSONBoolean target;

    std::ostringstream stream;
    target.SetValue(false);
    target.Serialize(stream);
    EXPECT_EQ("false", stream.str());
}

TEST_F(JSONBooleanTest, SerializeTrue)
{
    JSONBoolean target;

    std::ostringstream stream;
    target.SetValue(true);
    target.Serialize(stream);
    EXPECT_EQ("true", stream.str());
}

} // namespace json_parser

