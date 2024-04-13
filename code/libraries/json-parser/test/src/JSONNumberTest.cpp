#include "test-platform/GoogleTest.h"

#include <string>
#include "json-parser/JSONNumber.h"

template <class T>
std::ostream& PrintStandard(std::ostream& stream, std::streamsize /*integralDigits*/, std::streamsize fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    valuePrint << std::setfill('0') << std::setprecision(fractionalDigits) << value;
    stream << valuePrint.str();
    return stream;
}
template <class T>
std::ostream& PrintFixed(std::ostream& stream, std::streamsize integralDigits, std::streamsize fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    valuePrint << std::fixed << std::setfill('0') << std::setw(integralDigits + fractionalDigits + 1) << std::setprecision(fractionalDigits) << value;
    stream << valuePrint.str();
    return stream;
}
template <class T>
std::ostream& PrintScientific(std::ostream& stream, std::streamsize integralDigits, std::streamsize fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    valuePrint << std::scientific << std::setfill('0') << std::setw(integralDigits + fractionalDigits + 1) << std::setprecision(fractionalDigits) << value;
    stream << valuePrint.str();
    return stream;
}
template <class T>
std::string SerializeStandard(std::streamsize width, T value)
{
    std::ostringstream stream;
    PrintStandard(stream, 0, width, value);
    return stream.str();
}
template <class T>
std::string SerializeFixed(std::streamsize integralDigits, std::streamsize fractionalDigits, T value)
{
    std::ostringstream stream;
    PrintFixed(stream, integralDigits, fractionalDigits, value);
    return stream.str();
}
template <class T>
std::string SerializeScientific(std::streamsize integralDigits, std::streamsize fractionalDigits, T value)
{
    std::ostringstream stream;
    PrintScientific(stream, integralDigits, fractionalDigits, value);
    return stream.str();
}

namespace json_parser {
class JSONNumberTest: public ::testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JSONNumberTest, Construct)
{
    JSONNumber target;

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    int16_t int16Value{};
    EXPECT_FALSE(target.GetValue(int16Value));
    uint16_t uint16Value{};
    EXPECT_FALSE(target.GetValue(uint16Value));
    int32_t int32Value{};
    EXPECT_FALSE(target.GetValue(int32Value));
    uint32_t uint32Value{};
    EXPECT_FALSE(target.GetValue(uint32Value));
    int64_t int64Value{};
    EXPECT_FALSE(target.GetValue(int64Value));
    uint64_t uint64Value{};
    EXPECT_FALSE(target.GetValue(uint64Value));
    float floatValue{};
    EXPECT_FALSE(target.GetValue(floatValue));
    double doubleValue{};
    EXPECT_FALSE(target.GetValue(doubleValue));
}

TEST_F(JSONNumberTest, ConstructInitializerString)
{
    std::string value("123");
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ(value, target.GetValue());
    int8_t int8Value{};
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(123, int8Value);
    uint8_t uint8Value{};
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(123, uint8Value);
    int16_t int16Value{};
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(123, int16Value);
    uint16_t uint16Value{};
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(123, uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(123, int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(123U, uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(123, int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(123U, uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(123.0F, floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(123, doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerInt8)
{
    int8_t value = 123;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("123", target.GetValue());
    int8_t int8Value{};
    EXPECT_TRUE(target.GetValue(int8Value));
    EXPECT_EQ(static_cast<int8_t>(value), int8Value);
    uint8_t uint8Value{};
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(static_cast<uint8_t>(value), uint8Value);
    int16_t int16Value{};
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value{};
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerUInt8)
{
    uint8_t value = 234;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("234", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_TRUE(target.GetValue(uint8Value));
    EXPECT_EQ(static_cast<uint8_t>(value), uint8Value);
    int16_t int16Value{};
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value{};
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerInt16)
{
    int16_t value = 23456;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("23456", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value{};
    EXPECT_TRUE(target.GetValue(int16Value));
    EXPECT_EQ(static_cast<int16_t>(value), int16Value);
    uint16_t uint16Value{};
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerUInt16)
{
    uint16_t value = 34567;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("34567", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value{};
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value{};
    EXPECT_TRUE(target.GetValue(uint16Value));
    EXPECT_EQ(static_cast<uint16_t>(value), uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerInt32)
{
    int32_t value = 1234567890;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("1234567890", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value{};
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value{};
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value{};
    EXPECT_TRUE(target.GetValue(int32Value));
    EXPECT_EQ(static_cast<int32_t>(value), int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerUInt32)
{
    uint32_t value = 2345678901;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("2345678901", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value{};
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value{};
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value{};
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value{};
    EXPECT_TRUE(target.GetValue(uint32Value));
    EXPECT_EQ(static_cast<uint32_t>(value), uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerInt64)
{
    int64_t value = 4567890123;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("4567890123", target.GetValue());
    int8_t int8Value{};
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value{};
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value{};
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value{};
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value{};
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value{};
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0U, uint32Value);
    int64_t int64Value{};
    EXPECT_TRUE(target.GetValue(int64Value));
    EXPECT_EQ(static_cast<int64_t>(value), int64Value);
    uint64_t uint64Value{};
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue{};
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue{};
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerUInt64)
{
    uint64_t value = 9223372036854775808U;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ("9223372036854775808", target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0U, uint32Value);
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    uint64_t uint64Value;
    EXPECT_TRUE(target.GetValue(uint64Value));
    EXPECT_EQ(static_cast<uint64_t>(value), uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_EQ(static_cast<double>(value), doubleValue);
}

TEST_F(JSONNumberTest, ConstructInitializerFloat)
{
    float value = 1.2345E+23F;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ(SerializeStandard(8, value), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0U, uint32Value);
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    uint64_t uint64Value;
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0U, uint64Value);
    float floatValue;
    EXPECT_TRUE(target.GetValue(floatValue));
    EXPECT_EQ(static_cast<float>(value), floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(static_cast<double>(value), doubleValue, 0.00001E23);
}

TEST_F(JSONNumberTest, ConstructInitializerDouble)
{
    double value = 1.23456E+78;
    JSONNumber target(value);

    EXPECT_EQ(JSONValueType::Number, target.Type());
    EXPECT_EQ(SerializeStandard(16, value), target.GetValue());
    int8_t int8Value;
    EXPECT_FALSE(target.GetValue(int8Value));
    EXPECT_EQ(0, int8Value);
    uint8_t uint8Value;
    EXPECT_FALSE(target.GetValue(uint8Value));
    EXPECT_EQ(0, uint8Value);
    int16_t int16Value;
    EXPECT_FALSE(target.GetValue(int16Value));
    EXPECT_EQ(0, int16Value);
    uint16_t uint16Value;
    EXPECT_FALSE(target.GetValue(uint16Value));
    EXPECT_EQ(0, uint16Value);
    int32_t int32Value;
    EXPECT_FALSE(target.GetValue(int32Value));
    EXPECT_EQ(0, int32Value);
    uint32_t uint32Value;
    EXPECT_FALSE(target.GetValue(uint32Value));
    EXPECT_EQ(0U, uint32Value);
    int64_t int64Value;
    EXPECT_FALSE(target.GetValue(int64Value));
    EXPECT_EQ(0, int64Value);
    uint64_t uint64Value;
    EXPECT_FALSE(target.GetValue(uint64Value));
    EXPECT_EQ(0U, uint64Value);
    float floatValue;
    EXPECT_FALSE(target.GetValue(floatValue));
    EXPECT_EQ(0.0F, floatValue);
    double doubleValue;
    EXPECT_TRUE(target.GetValue(doubleValue));
    EXPECT_NEAR(static_cast<double>(value), doubleValue, 0.000001E78);
}

TEST_F(JSONNumberTest, Clear)
{
    double value = 1.23456E+78;
    JSONNumber target(value);

    EXPECT_FALSE(target.GetValue().empty());
    target.Clear();
    EXPECT_TRUE(target.GetValue().empty());
}

TEST_F(JSONNumberTest, SerializeUninitialized)
{
    JSONNumber target;

    std::ostringstream stream;
    target.Serialize(stream);
    EXPECT_EQ("", stream.str());
}

TEST_F(JSONNumberTest, SerializeInt8)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<int8_t>(-123));
    target.Serialize(stream);
    EXPECT_EQ("-123", stream.str());
}

TEST_F(JSONNumberTest, SerializeUInt8)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<uint8_t>(234));
    target.Serialize(stream);
    EXPECT_EQ("234", stream.str());
}

TEST_F(JSONNumberTest, SerializeInt16)
{
    JSONNumber target;

    std::ostringstream stream;
    target.Serialize(stream);
    target.SetValue(static_cast<int16_t>(-23456));
    target.Serialize(stream);
    EXPECT_EQ("-23456", stream.str());
}

TEST_F(JSONNumberTest, SerializeUInt16)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<uint16_t>(34567));
    target.Serialize(stream);
    EXPECT_EQ("34567", stream.str());
}

TEST_F(JSONNumberTest, SerializeInt32)
{
    JSONNumber target;

    std::ostringstream stream;
    target.Serialize(stream);
    target.SetValue(static_cast<int32_t>(-1234567890));
    target.Serialize(stream);
    EXPECT_EQ("-1234567890", stream.str());
}

TEST_F(JSONNumberTest, SerializeUInt32)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<uint32_t>(2345678901));
    target.Serialize(stream);
    EXPECT_EQ("2345678901", stream.str());
}

TEST_F(JSONNumberTest, SerializeInt64)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<int64_t>(-123456789012345));
    target.Serialize(stream);
    EXPECT_EQ("-123456789012345", stream.str());
}

TEST_F(JSONNumberTest, SerializeUInt64)
{
    JSONNumber target;

    std::ostringstream stream;
    target.SetValue(static_cast<uint64_t>(123456789012345));
    target.Serialize(stream);
    EXPECT_EQ("123456789012345", stream.str());
}

TEST_F(JSONNumberTest, SerializeFloat)
{
    JSONNumber target;

    float value = 123.456e23F;
    std::ostringstream stream;
    target.SetValue(value);
    target.Serialize(stream);
    EXPECT_EQ(SerializeStandard(8, value), stream.str());
}

TEST_F(JSONNumberTest, SerializeDouble)
{
    JSONNumber target;

    double value = 123.456789e123;
    std::ostringstream stream;
    target.SetValue(value);
    target.Serialize(stream);
    EXPECT_EQ(SerializeStandard(16, value), stream.str());
}

} // namespace json_parser

