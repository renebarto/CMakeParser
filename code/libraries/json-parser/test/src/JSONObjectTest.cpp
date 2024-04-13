#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/json.h"
#include "json-parser/JSONFactory.h"

namespace json_parser {

class JSONObjectTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONObjectTest, Construct)
{
    JSONObject target;

    EXPECT_EQ(JSONValueType::Object, target.Type());
    EXPECT_EQ(size_t {0}, target.Size());
}

TEST_F(JSONObjectTest, ConstructInitializerSinglePair)
{
    std::string key("key");
    JSONValuePtr value = CreateValue(true);

    JSONKVPair pair(key, value);
    JSONObject target({pair});

    EXPECT_EQ(JSONValueType::Object, target.Type());
    ASSERT_EQ(size_t {1}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key, (*iterator).GetKey());
    EXPECT_EQ(value, (*iterator).GetValue());
}

TEST_F(JSONObjectTest, ConstructInitializerMultiplePairs)
{
    std::string key1("key");
    JSONValuePtr value1 = CreateValue(true);
    std::string key2("key2");
    JSONValuePtr value2 = CreateValue("Some text");

    JSONObject target({{key1, value1}, {key2, value2}});

    EXPECT_EQ(JSONValueType::Object, target.Type());
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_F(JSONObjectTest, Clear)
{
    std::string key("key");
    JSONValuePtr value = CreateValue(true);

    JSONKVPair pair(key, value);
    JSONObject target({pair});

    ASSERT_EQ(size_t {1}, target.Size());
    target.Clear();
    ASSERT_EQ(size_t {0}, target.Size());
}

TEST_F(JSONObjectTest, AddPairDifferentKeys)
{
    JSONObject target;
    std::string key1("key");
    JSONValuePtr value1 = CreateValue(true);
    std::string key2("key2");
    JSONValuePtr value2 = CreateValue("Some text");
    ASSERT_EQ(size_t {0}, target.Size());
    EXPECT_TRUE(target.AddPair({key1, value1}));
    ASSERT_EQ(size_t {1}, target.Size());
    EXPECT_TRUE(target.AddPair(key2, value2));
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_F(JSONObjectTest, AddPairDuplicateKeys)
{
    JSONObject target;
    std::string key1("key");
    JSONValuePtr value1 = CreateValue(true);
    std::string key2("key2");
    JSONValuePtr value2 = CreateValue("Some text");
    ASSERT_EQ(size_t {0}, target.Size());
    EXPECT_TRUE(target.AddPair({key1, value1}));
    ASSERT_EQ(size_t {1}, target.Size());
    EXPECT_TRUE(target.AddPair(key2, value2));
    ASSERT_EQ(size_t {2}, target.Size());
    EXPECT_FALSE(target.AddPair({key2, value1}));
    ASSERT_EQ(size_t {2}, target.Size());
    auto iterator = target.GetIterator();
    EXPECT_EQ(key1, (*iterator).GetKey());
    EXPECT_EQ(value1, (*iterator).GetValue());
    ++iterator;
    EXPECT_EQ(key2, (*iterator).GetKey());
    EXPECT_EQ(value2, (*iterator).GetValue());
}

TEST_F(JSONObjectTest, Serialize)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    std::string key4("key4");
    std::string key5("key5");
    std::string key6("key6");
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue(false);
    JSONValuePtr value3 = std::make_shared<JSONNull>();
    JSONValuePtr value4 = std::make_shared<JSONNumber>("12345678");
    auto value5 = std::make_shared<JSONObject>();
    value5->AddPair(JSONKVPair("x", CreateValue(true)));
    auto value6 = std::make_shared<JSONArray>();
    value6->AddValue(CreateValue(true));
    value6->AddValue(CreateValue(false));

    JSONObject target(
    {
        { key1, value1 },
        { key2, value2 },
        { key3, value3 },
        { key4, value4 },
        { key5, value5 },
        { key6, value6 },
    });

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("{\n"
              "    \"" + key1 + "\" : true,\n"
              "    \"" + key2 + "\" : false,\n"
              "    \"" + key3 + "\" : null,\n"
              "    \"" + key4 + "\" : 12345678,\n"
              "    \"" + key5 + "\" : {\n"
              "        \"x\" : true\n"
              "    },\n"
              "    \"" + key6 + "\" : [\n"
              "        true,\n"
              "        false\n"
              "    ]\n"
              "}", stream.str());
}

TEST_F(JSONObjectTest, FindConst)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue(false);

    JSONObject target(
        {
            { key1, value1 },
            { key2, value2 },
        });
    const JSONObject constTarget = target;
    EXPECT_NOT_NULL(constTarget.Find(key1));
    EXPECT_NOT_NULL(constTarget.Find(key2));
    EXPECT_NULL(constTarget.Find(key3));
    // Careful, using std::dynamic_pointer_cast can easily cast away const-ness!
    auto valuePtr = constTarget.Find(key1);
    EXPECT_TRUE(std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->GetValue());
    std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->SetValue(false);
    EXPECT_FALSE(std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->GetValue());
}

TEST_F(JSONObjectTest, FindNonConst)
{
    std::string key1("key1");
    std::string key2("key2");
    std::string key3("key3");
    JSONValuePtr value1 = CreateValue(true);
    JSONValuePtr value2 = CreateValue(false);

    JSONObject target(
        {
            { key1, value1 },
            { key2, value2 },
        });
    EXPECT_NOT_NULL(target.Find(key1));
    EXPECT_NOT_NULL(target.Find(key2));
    EXPECT_NULL(target.Find(key3));
    auto valuePtr = target.Find(key1);
    EXPECT_TRUE(std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->GetValue());
    std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->SetValue(false);
    EXPECT_FALSE(std::dynamic_pointer_cast<JSONBoolean>(valuePtr)->GetValue());
}

} // namespace json_parser

