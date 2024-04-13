#include "json-parser/Parser.h"

#include "test-platform/GoogleTest.h"
#include "json-parser/json.h"

namespace json_parser {

class ParserTest
    : public ::testing::Test
{
public:
    std::string compilationUnit;

    ParserTest()
        : compilationUnit{ "ABC" }
    {
    }
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ParserTest, EmptyStream)
{
    std::istringstream stream("");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
}

TEST_F(ParserTest, SingleNull)
{
    std::istringstream stream("null");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Null, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNull());
    EXPECT_EQ("null", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNullEndingWithComma)
{
    std::istringstream stream("null,");
    Parser parser(compilationUnit, stream);

    // Parsing fails as we expect only a value, not a comma, however the value is extracted
    EXPECT_FALSE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Null, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNull());
    EXPECT_EQ("null", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleTrue)
{
    std::istringstream stream("true");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Boolean, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsBoolean());
    EXPECT_EQ("true", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleFalse)
{
    std::istringstream stream("false");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Boolean, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsBoolean());
    EXPECT_EQ("false", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumber)
{
    std::istringstream stream("123");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberFixedPoint)
{
    std::istringstream stream("123.456");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123.456", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberFixedPointNoFraction)
{
    std::istringstream stream("123.");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123.", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberFixedPointOnlyFraction)
{
    std::istringstream stream(".456");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ(".456", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberScientific)
{
    std::istringstream stream("123.456E789");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123.456E789", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberScientificPosExp)
{
    std::istringstream stream("123.456E+789");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123.456E+789", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleNumberScientificNegExp)
{
    std::istringstream stream("123.456e-789");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Number, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsNumber());
    EXPECT_EQ("123.456e-789", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleStringEmpty)
{
    std::istringstream stream("\"\"");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::String, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsString());
    EXPECT_EQ("\"\"", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleStringNoQuotes)
{
    std::istringstream stream("\"ABC DEF\"");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::String, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsString());
    EXPECT_EQ("\"ABC DEF\"", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseSingleStringWithQuotes)
{
    std::istringstream stream("\"ABC \\\"DEF\\\"\"");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::String, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsString());
    EXPECT_EQ("\"ABC \\\"DEF\\\"\"", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseEmptyArray)
{
    std::istringstream stream("[]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsArray());
    EXPECT_EQ(size_t{ 0 }, parser.GetValue()->AsArray()->Size());
    EXPECT_EQ("[\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseArrayWithSingleValue)
{
    std::istringstream stream("[true]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsArray());
    EXPECT_EQ(size_t{ 1 }, parser.GetValue()->AsArray()->Size());
    EXPECT_EQ("[\n    true\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseArrayWithSingleValueWithComma)
{
    std::istringstream stream("[true,]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsArray());
    EXPECT_EQ(size_t{ 1 }, parser.GetValue()->AsArray()->Size());
    EXPECT_EQ("[\n    true\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseArrayWithMultipleValues)
{
    std::istringstream stream("[true,false,null]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsArray());
    EXPECT_EQ(size_t{ 3 }, parser.GetValue()->AsArray()->Size());
    EXPECT_EQ("[\n    true,\n    false,\n    null\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseArrayWithMultipleValuesEmbeddedArray)
{
    std::istringstream stream("[true,false,[null]]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsArray());
    EXPECT_EQ(size_t{ 3 }, parser.GetValue()->AsArray()->Size());
    EXPECT_EQ("[\n    true,\n    false,\n    [\n        null\n    ]\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseEmptyObject)
{
    std::istringstream stream("{}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsObject());
    EXPECT_EQ(size_t{ 0 }, parser.GetValue()->AsObject()->Size());
    EXPECT_EQ("{\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseObjectSingleKVPair)
{
    std::istringstream stream("{\"a\":null}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsObject());
    EXPECT_EQ(size_t{ 1 }, parser.GetValue()->AsObject()->Size());
    EXPECT_EQ("{\n    \"a\" : null\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseObjectSingleKVPairComma)
{
    std::istringstream stream("{\"a\":null,}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsObject());
    EXPECT_EQ(size_t{ 1 }, parser.GetValue()->AsObject()->Size());
    EXPECT_EQ("{\n    \"a\" : null\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseObjectSingleStringComma)
{
    std::istringstream stream("{\"a\":\"b\",}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsObject());
    EXPECT_EQ(size_t{ 1 }, parser.GetValue()->AsObject()->Size());
    EXPECT_EQ("{\n    \"a\" : \"b\"\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseObjectMultipleKVPair)
{
    std::istringstream stream("{\"a\":null,\"b\":true,\"c\":false}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    EXPECT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    ASSERT_NOT_NULL(parser.GetValue()->AsObject());
    EXPECT_EQ(size_t{ 3 }, parser.GetValue()->AsObject()->Size());
    EXPECT_EQ("{\n    \"a\" : null,\n    \"b\" : true,\n    \"c\" : false\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseArrayWithEmptyObject)
{
    std::istringstream stream("[{}]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    ASSERT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    auto arrayValue = parser.GetValue()->AsArray();
    ASSERT_EQ(size_t{ 1 }, arrayValue->Size());
    auto arrayIt = arrayValue->GetIterator();
    ASSERT_FALSE(arrayIt.AtEnd());
    ASSERT_EQ(JSONValueType::Object, (*arrayIt)->Type());
    auto objectValue = (*arrayIt)->AsObject();
    ASSERT_EQ(size_t{ 0 }, objectValue->Size());
    arrayIt++;
    ASSERT_TRUE(arrayIt.AtEnd());
    EXPECT_EQ("[\n    {\n    }\n]", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseObjectWithEmptyArray)
{
    std::istringstream stream("{\"a\":[]}");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    ASSERT_EQ(JSONValueType::Object, parser.GetValue()->Type());
    auto objectValue = parser.GetValue()->AsObject();
    ASSERT_EQ(size_t{ 1 }, objectValue->Size());
    auto objectIt = objectValue->GetIterator();
    ASSERT_FALSE(objectIt.AtEnd());
    ASSERT_EQ("a", (*objectIt).GetKey());
    ASSERT_EQ(JSONValueType::Array, (*objectIt).GetValue()->Type());
    auto arrayValue = (*objectIt).GetValue()->AsArray();
    ASSERT_EQ(size_t{ 0 }, arrayValue->Size());
    objectIt++;
    ASSERT_TRUE(objectIt.AtEnd());
    EXPECT_EQ("{\n    \"a\" : [\n    ]\n}", serialization::Serialize(parser.GetValue()));
}

TEST_F(ParserTest, ParseComplex)
{
    std::istringstream stream(
        "["
        "  {"
        "    \"type\": \"TestSynth\","
        "    \"polyphony\": 1,"
        "  }"
        "]");
    Parser parser(compilationUnit, stream);

    EXPECT_TRUE(parser.Parse());
    ASSERT_NOT_NULL(parser.GetValue());
    ASSERT_EQ(JSONValueType::Array, parser.GetValue()->Type());
    auto arrayValue = parser.GetValue()->AsArray();
    ASSERT_EQ(size_t{ 1 }, arrayValue->Size());
    auto arrayIt = arrayValue->GetIterator();
    ASSERT_FALSE(arrayIt.AtEnd());
    ASSERT_EQ(JSONValueType::Object, (*arrayIt)->Type());
    auto objectValue = (*arrayIt)->AsObject();
    ASSERT_EQ(size_t{ 2 }, objectValue->Size());
    auto item1 = objectValue->GetString("type");
    ASSERT_NOT_NULL(item1);
    EXPECT_EQ("TestSynth", item1->GetValue());
    auto item2 = objectValue->GetNumber("polyphony");
    ASSERT_NOT_NULL(item2);
    EXPECT_EQ("1", item2->GetValue());
    arrayIt++;
    ASSERT_TRUE(arrayIt.AtEnd());
    EXPECT_EQ("[\n    {\n        \"polyphony\" : 1,\n        \"type\" : \"TestSynth\"\n    }\n]", serialization::Serialize(parser.GetValue()));
}

} // namespace json_parser
