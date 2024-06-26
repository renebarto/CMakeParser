//------------------------------------------------------------------------------
// Copyright   : Copyright(c) 2021 Ren� Barto
//
// File        : SerializationTest.cpp
//
// Namespace   : serialization
//
// Class       : -
//
// Description :
//
//------------------------------------------------------------------------------

#include "test-platform/GoogleTest.h"

#include <cstring>
#include "utility/Serialization.h"

namespace serialization {

template <class T>
static std::ostream& PrintStandard(std::ostream& stream, int /*integralDigits*/, int fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    valuePrint << std::setfill('0') << std::setprecision(fractionalDigits) << value;
    stream << valuePrint.str();
    return stream;
}

template <class T>
static std::ostream& PrintScientific(std::ostream& stream, int integralDigits, int fractionalDigits, T value)
    {
    std::ostringstream valuePrint;
    valuePrint << std::scientific << std::setfill('0') 
        << std::setw(integralDigits + fractionalDigits + 1)
        << std::setprecision(fractionalDigits) << value;
    stream << valuePrint.str();
    return stream;
}

template <class T>
static std::string SerializeStandard(int width, T value)
{
    std::ostringstream stream;
    PrintStandard(stream, 0, width, value);
    return stream.str();
}

template <class T>
static std::string SerializeScientific(int integralDigits, int fractionalDigits, T value)
{
    std::ostringstream stream;
    PrintScientific(stream, integralDigits, fractionalDigits, value);
    return stream.str();
}

TEST(SerializationTest, SerializeBool)
{
    bool value = true;
    EXPECT_EQ("true", Serialize(value));
    value = false;
    EXPECT_EQ("   false", Serialize(value, 8));
}

TEST(SerializationTest, SerializeInt8)
{
    int8_t value = 0x55;
    EXPECT_EQ("01010101", Serialize(value, 0, 2));
    EXPECT_EQ("125", Serialize(value, 0, 8));
    EXPECT_EQ("85", Serialize(value, 0, 10));
    EXPECT_EQ("55", Serialize(value, 0, 16));
    EXPECT_EQ("0x55", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeUInt8)
{
    uint8_t value = 0xAA;
    EXPECT_EQ("10101010", Serialize(value, 0, 2));
    EXPECT_EQ("252", Serialize(value, 0, 8));
    EXPECT_EQ("170", Serialize(value, 0, 10));
    EXPECT_EQ("AA", Serialize(value, 0, 16));
    EXPECT_EQ("0xAA", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeInt16)
{
    int16_t value = 0x5555;
    EXPECT_EQ("0101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("052525", Serialize(value, 0, 8));
    EXPECT_EQ("21845", Serialize(value, 0, 10));
    EXPECT_EQ("5555", Serialize(value, 0, 16));
    EXPECT_EQ("0x5555", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeUInt16)
{
    uint16_t value = 0xAAAA;
    EXPECT_EQ("1010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("125252", Serialize(value, 0, 8));
    EXPECT_EQ("43690", Serialize(value, 0, 10));
    EXPECT_EQ("AAAA", Serialize(value, 0, 16));
    EXPECT_EQ("0xAAAA", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeInt32)
{
    int32_t value = 0x55555555;
    EXPECT_EQ("01010101010101010101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("12525252525", Serialize(value, 0, 8));
    EXPECT_EQ("1431655765", Serialize(value, 0, 10));
    EXPECT_EQ("55555555", Serialize(value, 0, 16));
    EXPECT_EQ("0x55555555", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeUInt32)
{
    uint32_t value = 0xAAAAAAAA;
    EXPECT_EQ("10101010101010101010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("25252525252", Serialize(value, 0, 8));
    EXPECT_EQ("2863311530", Serialize(value, 0, 10));
    EXPECT_EQ("AAAAAAAA", Serialize(value, 0, 16));
    EXPECT_EQ("0xAAAAAAAA", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeInt64)
{
    int64_t value = 0x5555555555555555;
    EXPECT_EQ("0101010101010101010101010101010101010101010101010101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("0525252525252525252525", Serialize(value, 0, 8));
    EXPECT_EQ("6148914691236517205", Serialize(value, 0, 10));
    EXPECT_EQ("5555555555555555", Serialize(value, 0, 16));
    EXPECT_EQ("0x5555555555555555", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeUInt64)
{
    uint64_t value = 0xAAAAAAAAAAAAAAAA;
    EXPECT_EQ("1010101010101010101010101010101010101010101010101010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("1252525252525252525252", Serialize(value, 0, 8));
    EXPECT_EQ("12297829382473034410", Serialize(value, 0, 10));
    EXPECT_EQ("AAAAAAAAAAAAAAAA", Serialize(value, 0, 16));
    EXPECT_EQ("0xAAAAAAAAAAAAAAAA", Serialize(value, 0, 16, true));
}

TEST(SerializationTest, SerializeFloat)
{
    float value = 1.0002e4F;
    EXPECT_EQ("10002", Serialize(value));
    EXPECT_EQ("   10002", Serialize(value, 8));
    EXPECT_EQ("10002.0000", Serialize(value, 0, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ("0010002.0000", Serialize(value, 12, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ(SerializeScientific(0, 4, value), Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
    EXPECT_EQ("1.0002e+04", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
    EXPECT_EQ("10002", Serialize(value, 0, 8, FloatingPointRepresentation::Mixed));
    value = 5.0E-37F;
    EXPECT_EQ(SerializeStandard(16, value), Serialize(value));
   EXPECT_EQ("5.000000179695649e-37", Serialize(value));
   EXPECT_EQ("5e-37", Serialize(value, 0, 1));
}

TEST(SerializationTest, SerializeDouble)
{
    double value = 1.00002E05;
    EXPECT_EQ("100002", Serialize(value));
    EXPECT_EQ("  100002", Serialize(value, 8));
    EXPECT_EQ("100002.0000", Serialize(value, 0, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ("0100002.0000", Serialize(value, 12, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ(SerializeScientific(0, 4, value), Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
    EXPECT_EQ("1.0000e+05", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
    EXPECT_EQ("100002", Serialize(value, 0, 8, FloatingPointRepresentation::Mixed));
    value = 1e-200;
    EXPECT_EQ("1e-200", Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Serialize(value));
    EXPECT_EQ("1e-200", Serialize(value, 0, 2));
}

TEST(SerializationTest, SerializeStringNarrow)
{
    std::string value = "Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeStringNarrowQuoted)
{
    std::string value = "Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

TEST(SerializationTest, SerializeCharPtrNarrow)
{
    const char * value = "Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeCharPtrNarrowQuoted)
{
    const char * value = "Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

TEST(SerializationTest, SerializeConstVoidPtrNull)
{
    const void* value = nullptr;
    std::string expected = "null";
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeConstVoidPtrNullWidth)
{
    const void * value = nullptr;
    std::string expected = "    null";
    EXPECT_EQ(expected, Serialize(value, 8));
}

TEST(SerializationTest, SerializeConstVoidPtr)
{
    const void * value = reinterpret_cast<const void *>(0x123456789ABCDEF0llu);
    std::string expected;
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(value) == 4)
    {
        expected = "0x9ABCDEF0";
    }
    else
    {
        expected = "0x123456789ABCDEF0";
    }
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeConstVoidPtrWidth)
{
    const void* value = reinterpret_cast<const void*>(0x123456789ABCDEF0llu);
    std::string expected;
    //TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(value) == 4)
    {
        expected = "0x9ABCDEF0";
    }
    else
    {
        expected = "0x123456789ABCDEF0";
    }
    //TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    EXPECT_EQ(expected, Serialize(value, 0));
}

TEST(SerializationTest, SerializeNonConstVoidPtr)
{
    void* value = reinterpret_cast<void*>(0x123456789ABCDEF0llu);
    std::string expected;
    //TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(value) == 4)
    {
        expected = "0x9ABCDEF0";
    }
    else
    {
        expected = "0x123456789ABCDEF0";
    }
    //TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeNonConstVoidPtrWidth)
{
    void * value = reinterpret_cast<void *>(0x123456789ABCDEF0llu);
    std::string expected;
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(value) == 4)
    {
        expected = "0x9ABCDEF0";
    }
    else
    {
        expected = "0x123456789ABCDEF0";
    }
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    EXPECT_EQ(expected, Serialize(value, 0));
}

TEST(SerializationTest, SerializeUInt8PtrNull)
{
    const std::uint8_t * value = nullptr;
    std::string expected = "null";
    EXPECT_EQ(expected, SerializeData(value, std::size_t {0}));
}

TEST(SerializationTest, SerializeUInt8PtrZeroLength)
{
    const std::uint8_t value[] { 0x00 };
    std::string expected = "\n";
    EXPECT_EQ(expected, SerializeData(value, std::size_t {0}));
}

TEST(SerializationTest, SerializeUInt8PtrOneByte)
{
    const std::uint8_t value[] { 0x41 };
    
    std::string expected = "\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000000  41                                               A                              \n";
    EXPECT_EQ(expected, SerializeData(value, sizeof(value)));
}

TEST(SerializationTest, SerializeUInt8PtrFifteenBytes)
{
    const std::uint8_t value[] { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F };
    std::string expected = "\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000000  41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F     A B C D E F G H I J K L M N O  \n";
    EXPECT_EQ(expected, SerializeData(value, sizeof(value)));
}

TEST(SerializationTest, SerializeUInt8PtrSixteenBytes)
{
    const std::uint8_t value[] { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50 };
    std::string expected = "\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000000  41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50  A B C D E F G H I J K L M N O P\n";
    EXPECT_EQ(expected, SerializeData(value, sizeof(value)));
}

TEST(SerializationTest, SerializeUInt8PtrThirtyTwoBytesWithNonPrintables)
{
    const std::uint8_t value[] { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
                                 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20 };
    std::string expected = "\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000000  41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50  A B C D E F G H I J K L M N O P\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000010  11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20  . . . . . . . . . . . . . . .  \n";
    EXPECT_EQ(expected, SerializeData(value, sizeof(value)));
}

TEST(SerializationTest, SerializeVectorEmpty)
{
    const std::vector<std::uint8_t> value;
    std::string expected = "-";
    EXPECT_EQ(expected, SerializeData(value));
}

TEST(SerializationTest, SerializeVectorThirtyTwoBytesWithNonPrintables)
{
    const std::vector<std::uint8_t> value = { 
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
    };
    std::string expected = "\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000000  41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50  A B C D E F G H I J K L M N O P\n";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(disable : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037 Expression is constant
#endif
//TICS +PRE-POR#021
    if (sizeof(std::size_t) == 8)
        expected += "00000000";
//TICS -PRE-POR#021 We suppress warnings for Windows only
#if defined(PLATFORM_WINDOWS)
#pragma warning(default : 4127) //TICS !POR#018 !POR#037 !PRE-POR#037
#endif
//TICS +PRE-POR#021
    expected += "00000010  11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20  . . . . . . . . . . . . . . .  \n";
    EXPECT_EQ(expected, SerializeData(value));
}

TEST(SerializationTest, SerializeVectorOfUInt8)
{
    const std::vector<std::uint8_t> value = { 
        1, 2, 3, 4
    };
    std::string expected = "[0x01,0x02,0x03,0x04]";
    EXPECT_EQ(expected, Serialize(value));
}

TEST(SerializationTest, SerializeVectorOfInts)
{
    const std::vector<int> value = { 
        1, 2, 3, 4
    };
    std::string expected = "[1,2,3,4]";
    EXPECT_EQ(expected, Serialize(value));
}

} // namespace serialization
