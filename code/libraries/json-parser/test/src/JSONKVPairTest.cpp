#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/json.h"

namespace json_parser {

class JSONKVPairTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONKVPairTest, Construct)
{
    JSONKVPair target;

    EXPECT_EQ("", target.GetKey());
    EXPECT_NULL(target.GetValue());
}

TEST_F(JSONKVPairTest, ConstructInitializer)
{
    std::string key("key");
    JSONValuePtr value = std::make_shared<JSONBoolean>(true);
    JSONKVPair target(key, value);

    EXPECT_EQ(key, target.GetKey());
    ASSERT_NOT_NULL(target.GetValue());
    ASSERT_EQ(JSONValueType::Boolean, target.GetValue()->Type());
    EXPECT_TRUE(std::dynamic_pointer_cast<JSONBoolean>(target.GetValue())->GetValue());
}

TEST_F(JSONKVPairTest, SerializeKeyBoolean)
{
    std::string key("key");
    JSONValuePtr value = std::make_shared<JSONBoolean>(true);
    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : true", stream.str());
}

TEST_F(JSONKVPairTest, SerializeKeyNumber)
{
    std::string key("key");
    std::shared_ptr<JSONNumber> value = std::make_shared<JSONNumber>();
    value->SetValue(12345678);

    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : 12345678", stream.str());
}

TEST_F(JSONKVPairTest, SerializeKeyString)
{
    std::string key("key");
    JSONValuePtr value = std::make_shared<JSONString>("Some text");
    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : \"Some text\"", stream.str());
}

TEST_F(JSONKVPairTest, SerializeKeyNoValue)
{
    std::string key("key");
    JSONValuePtr value = nullptr;
    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : null", stream.str());
}

TEST_F(JSONKVPairTest, SerializeObject)
{
    std::string key("key");
    std::shared_ptr<JSONObject> value = std::make_shared<JSONObject>();
    std::string keySub("keySub");
    JSONValuePtr valueSub = std::make_shared<JSONBoolean>(true);
    JSONKVPair targetSub(keySub, valueSub);
    value->AddPair(targetSub);
    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : {\n    \"" + keySub + "\" : true\n" + "}", stream.str());
}

TEST_F(JSONKVPairTest, SerializeArray)
{
    std::string key("key");
    std::shared_ptr<JSONArray> value = std::make_shared<JSONArray>();
    JSONValuePtr valueSub = std::make_shared<JSONBoolean>(true);
    value->AddValue(valueSub);
    JSONKVPair target(key, value);

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("\"" + key + "\" : [\n    true\n" + "]", stream.str());
}

} // namespace json_parser

