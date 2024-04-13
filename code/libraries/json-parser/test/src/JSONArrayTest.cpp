#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/json.h"
#include "json-parser/JSONFactory.h"

namespace json_parser {

class JSONArrayTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONArrayTest, Construct)
{
    JSONArray target;

    EXPECT_EQ(JSONValueType::Array, target.Type());
    EXPECT_EQ(size_t {0}, target.Size());
}

TEST_F(JSONArrayTest, ConstructInitializerSingleValue)
{
    JSONValuePtr value = CreateValue(true);

    JSONArray target({value});

    EXPECT_EQ(JSONValueType::Array, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value, (*iterator));
}

TEST_F(JSONArrayTest, ConstructInitializerMultipleValues)
{
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue("Some text");

    JSONArray target({value1, value2});

    EXPECT_EQ(JSONValueType::Array, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value1, (*iterator));
    ++iterator;
    EXPECT_EQ(value2, (*iterator));
}

TEST_F(JSONArrayTest, Clear)
{
    JSONValuePtr value = CreateValue(true);

    JSONArray target({value});

    ASSERT_EQ(size_t {1}, target.Size());
    target.Clear();
    ASSERT_EQ(size_t {0}, target.Size());
}

TEST_F(JSONArrayTest, AddValue)
{
    JSONArray target;
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue("Some text");
    ASSERT_EQ(size_t {0}, target.Size());
    target.AddValue(value1);
    ASSERT_EQ(size_t {1}, target.Size());
    target.AddValue(value2);
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(value1, (*iterator));
    ++iterator;
    EXPECT_EQ(value2, (*iterator));
}

TEST_F(JSONArrayTest, Serialize)
{
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue(false);
    JSONValuePtr value3 = std::make_shared<JSONNull>();
    JSONValuePtr value4 = std::make_shared<JSONNumber>("12345678");
    auto value5 = std::make_shared<JSONObject>();
    value5->AddPair(JSONKVPair("x", CreateValue(true)));
    auto value6 = std::make_shared<JSONArray>();
    value6->AddValue(CreateValue(true));
    value6->AddValue(CreateValue(false));

    JSONArray target(
    {
        value1,
        value2,
        value3,
        value4,
        value5,
        value6
    });

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("[\n"
              "    true,\n"
              "    false,\n"
              "    null,\n"
              "    12345678,\n"
              "    {\n"
              "        \"x\" : true\n"
              "    },\n"
              "    [\n"
              "        true,\n"
              "        false\n"
              "    ]\n"
              "]", stream.str());
}

TEST_F(JSONArrayTest, Vector)
{
    std::vector<bool> data { true, false, true };

    auto target = CreateArray(data);

    std::ostringstream stream;
    target->Serialize(stream);
    EXPECT_EQ("[\n"
              "    true,\n"
              "    false,\n"
              "    true\n"
              "]", stream.str());
}

} // namespace json_parser

