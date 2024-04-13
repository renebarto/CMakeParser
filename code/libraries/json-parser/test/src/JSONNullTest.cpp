#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/JSONNull.h"

namespace json_parser {

class JSONNullTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONNullTest, Construct)
{
    JSONNull target;

    EXPECT_EQ(JSONValueType::Null, target.Type());
}

TEST_F(JSONNullTest, Serialize)
{
    JSONNull target;

    EXPECT_EQ(JSONValueType::Null, target.Type());
    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("null", stream.str());
}

} // namespace json_parser

