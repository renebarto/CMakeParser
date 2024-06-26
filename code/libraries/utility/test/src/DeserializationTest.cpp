#include "test-platform/GoogleTest.h"

#include "utility/Deserialization.h"

namespace serialization {

TEST(DeserializationTest, DeserializeBool)
{
    bool expected = true;
    bool actual {};
    EXPECT_TRUE(Deserialize("true", actual));
    EXPECT_EQ(expected, actual);

    expected = false;
    EXPECT_TRUE(Deserialize("FaLsE", actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("abcd", actual));
    EXPECT_FALSE(Deserialize("1234", actual));
}

TEST(DeserializationTest, DeserializeInt8)
{
    int8_t expected = 127;
    int8_t actual {};
    EXPECT_TRUE(Deserialize("127", actual));
    EXPECT_EQ(expected, actual);

    expected = 0x55;
    EXPECT_TRUE(Deserialize("55", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("1234", actual));
}

TEST(DeserializationTest, DeserializeUInt8)
{
    uint8_t expected = 254;
    uint8_t actual {};
    EXPECT_TRUE(Deserialize("254", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xab;
    EXPECT_TRUE(Deserialize("aB", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("1234", actual));
}

TEST(DeserializationTest, DeserializeInt16)
{
    int16_t expected = 12345;
    int16_t actual {};
    EXPECT_TRUE(Deserialize("12345", actual));
    EXPECT_EQ(expected, actual);

//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
    #pragma warning(disable : 4309) //TICS !POR#018 !POR#037 !PRE-POR#037 truncation of constant value
#endif
    expected = static_cast<int16_t>(0xabcd);
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
    #pragma warning(default : 4309) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
    EXPECT_TRUE(Deserialize("aBcD", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("54321", actual));
}

TEST(DeserializationTest, DeserializeUInt16)
{
    uint16_t expected = 54321;
    uint16_t actual {};
    EXPECT_TRUE(Deserialize("54321", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcd;
    EXPECT_TRUE(Deserialize("aBcD", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("654321", actual));
}

TEST(DeserializationTest, DeserializeInt32)
{
    int32_t expected = 12345678;
    int32_t actual {};
    EXPECT_TRUE(Deserialize("12345678", actual));
    EXPECT_EQ(expected, actual);

    expected = static_cast<int32_t>(0x7bcdefef);
    EXPECT_TRUE(Deserialize("7BcDEfeF", actual, 16));
    EXPECT_EQ(expected, actual);

    // Depending on platform, this conversion with fail and return 0 or succeed
    // On Windows 64-bit and ARM platforms it fails
    // On Linux 64 bit it succeeds
    auto result = Deserialize("aBcDEfeF", actual, 16);
    if (result)
        expected = -1412567057;
    else
        expected = 0;
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("FFFFFFFFF", actual));
}

TEST(DeserializationTest, DeserializeUInt32)
{
    uint32_t expected = 4000000000u;
    uint32_t actual {};
    EXPECT_TRUE(Deserialize("4000000000", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcdefef;
    EXPECT_TRUE(Deserialize("aBcDeFeF", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("FFFFFFFFF", actual));
}

TEST(DeserializationTest, DeserializeInt64)
{
    int64_t expected = 987654321;
    int64_t actual {};
    EXPECT_TRUE(Deserialize("987654321", actual));
    EXPECT_EQ(expected, actual);

    expected = 0x7edcba9876543210;
    EXPECT_TRUE(Deserialize("7EdCbA9876543210", actual, 16));
    EXPECT_EQ(expected, actual);

    expected = 0;
    EXPECT_FALSE(Deserialize("fEdCbA9876543210", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("FFFFFFFFFFFFFFFFF", actual));
}

TEST(DeserializationTest, DeserializeUInt64)
{
    uint64_t expected = 9223372036854775808ULL;
    uint64_t actual {};
    EXPECT_TRUE(Deserialize("9223372036854775808", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xfedcba9876543210;
    EXPECT_TRUE(Deserialize("fEdCbA9876543210", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize("xyz", actual));
    EXPECT_FALSE(Deserialize("FFFFFFFFFFFFFFFFF", actual));
}

TEST(DeserializationTest, DeserializeFloat)
{
    float expected = 1002;
    float actual {};
    EXPECT_TRUE(Deserialize("1.002E+03", actual));
    EXPECT_NEAR(expected, actual, 0.0F);

    expected = 5.0E-37F;
    EXPECT_TRUE(Deserialize("5e-37", actual));
    EXPECT_NEAR(expected, actual, 0.0F);

    EXPECT_FALSE(Deserialize("xyz", actual));
}

TEST(DeserializationTest, DeserializeDouble)
{
    double expected = 1000002;
    double actual {};
    EXPECT_TRUE(Deserialize("1.000002E+06", actual));
    EXPECT_NEAR(expected, actual, 0.0);

    expected = 1e-200;
    EXPECT_TRUE(Deserialize("1e-200", actual));
    EXPECT_NEAR(expected, actual, 0.0);

    EXPECT_FALSE(Deserialize("xyz", actual));
}

TEST(DeserializationTest, DeserializeString)
{
    std::string expected = "Hello World";
    std::string actual {};
    EXPECT_TRUE(Deserialize("Hello World", actual));
    EXPECT_EQ(expected, actual);
}

} // namespace serialization
