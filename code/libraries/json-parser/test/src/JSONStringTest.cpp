#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/JSONString.h"

namespace json_parser {

class JSONStringTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONStringTest, Construct)
{
    JSONString target;

    EXPECT_EQ(JSONValueType::String, target.Type());
    EXPECT_EQ("", target.GetValue());
}

TEST_F(JSONStringTest, ConstructInitializer)
{
    std::string value("String");
    JSONString target(value);

    EXPECT_EQ(JSONValueType::String, target.Type());
    EXPECT_EQ(value, target.GetValue());
}

TEST_F(JSONStringTest, Clear)
{
    std::string value("String");
    JSONString target(value);

    EXPECT_EQ(value, target.GetValue());
    target.Clear();
    EXPECT_EQ("", target.GetValue());
}

TEST_F(JSONStringTest, SerializeEmptyString)
{
    JSONString target;

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"\"", stream.str());
}

TEST_F(JSONStringTest, SerializeString)
{
    JSONString target;

    std::ostringstream stream;
    target.SetValue("Some text");
    target.Serialize(stream);
    EXPECT_EQ("\"Some text\"", stream.str());
}

} // namespace json_parser

