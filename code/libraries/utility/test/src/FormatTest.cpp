#include "test-platform/GoogleTest.h"

#include "utility/Format.h"
#include "serialization/BidirectionalMap.h"
#include "serialization/EnumSerialization.h"

namespace utility {

template <class T>
std::string FormatStandard(int width, int /*integralDigits*/, int /*fractionalDigits*/, T value)
{
    std::ostringstream valuePrint;
    std::ostringstream stream;
    valuePrint << std::setfill('0') << std::setprecision(width) << value;
    if (static_cast<size_t>(width) > valuePrint.str().length())
        stream << std::string(static_cast<size_t>(width) - valuePrint.str().length(), ' ');
    stream << valuePrint.str();
    return stream.str();
}
template <class T>
std::string FormatFixed(int width, int integralDigits, int fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    std::ostringstream stream;
    valuePrint << std::fixed << std::setfill('0') << std::setw(integralDigits + fractionalDigits + 1) << std::setprecision(fractionalDigits) << value;
    if (static_cast<size_t>(width) > valuePrint.str().length())
        stream << std::string(static_cast<size_t>(width) - valuePrint.str().length(), ' ');
    stream << valuePrint.str();
    return stream.str();
}
template <class T>
std::string FormatScientific(int width, int integralDigits, int fractionalDigits, T value)
{
    std::ostringstream valuePrint;
    std::ostringstream stream;
    valuePrint << std::scientific << std::setfill('0') << std::setw(integralDigits + fractionalDigits + 1) << std::setprecision(fractionalDigits) << value;
    if (static_cast<size_t>(width) > valuePrint.str().length())
        stream << std::string(static_cast<size_t>(width) - valuePrint.str().length(), ' ');
    stream << valuePrint.str();
    return stream.str();
}

enum class FormatTestEnum { Yes, No };

} // namespace utility

namespace serialization {

template<>
const serialization::BidirectionalMap<utility::FormatTestEnum, std::string> EnumSerializationMap<utility::FormatTestEnum>::ConversionMap = 
{
    { utility::FormatTestEnum::Yes, "Yes"},
    { utility::FormatTestEnum::No, "No"},
};

} // namespace serialization

namespace utility {

inline std::ostream& operator << (std::ostream& stream, FormatTestEnum value)
{
    return stream << serialization::Serialize(value);
}

TEST(BasicFormatTest, FormatCharPtr)
{
    const char* text = "Hello";

    std::string str = BasicFormat(text, 0, "");
    EXPECT_EQ("Hello", str);

    str = BasicFormat(text, -10, "");
    EXPECT_EQ("Hello     ", str);

    str = BasicFormat(text, 10, "");
    EXPECT_EQ("     Hello", str);
}

TEST(BasicFormatTest, FormatString)
{
    std::string text = "Hello";

    std::string str = BasicFormat(text, 0, "");
    EXPECT_EQ("Hello", str);

    str = BasicFormat(text, -10, "");
    EXPECT_EQ("Hello     ", str);

    str = BasicFormat(text, 10, "");
    EXPECT_EQ("     Hello", str);
}

TEST(BasicFormatTest, FormatEnum)
{
    FormatTestEnum value = FormatTestEnum::Yes;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("Yes", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("Yes       ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("       Yes", str);
}

TEST(BasicFormatTest, FormatInt8)
{
    std::int8_t value = 123;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("123", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("123       ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("       123", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("123", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("     123", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("01111011", str);

    // Binary string for 8 bit int is always 8 characters long
    str = BasicFormat(value, 0, "B11");
    EXPECT_EQ("01111011", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("173", str);

    str = BasicFormat(value, 0, "O8");
    EXPECT_EQ("00000173", str);

    str = BasicFormat(value, 10, "O8");
    EXPECT_EQ("  00000173", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("7B", str);

    str = BasicFormat(value, 0, "X4");
    EXPECT_EQ("007B", str);

    str = BasicFormat(value, 10, "X4");
    EXPECT_EQ("      007B", str);
}

TEST(BasicFormatTest, FormatUInt8)
{
    uint8_t value = 123;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("123", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("123       ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("       123", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("123", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("     123", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("01111011", str);

    // Binary string for 8 bit int is always 8 characters long
    str = BasicFormat(value, 0, "B11");
    EXPECT_EQ("01111011", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("173", str);

    str = BasicFormat(value, 0, "O8");
    EXPECT_EQ("00000173", str);

    str = BasicFormat(value, 10, "O8");
    EXPECT_EQ("  00000173", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("7B", str);

    str = BasicFormat(value, 0, "X4");
    EXPECT_EQ("007B", str);

    str = BasicFormat(value, 10, "X4");
    EXPECT_EQ("      007B", str);
}

TEST(BasicFormatTest, FormatInt16)
{
    int16_t value = 12345;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("12345", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("12345     ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("     12345", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("12345", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("   12345", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("0011000000111001", str);

    // Binary string for 16 bit int is always 16 characters long
    str = BasicFormat(value, 0, "B23");
    EXPECT_EQ("0011000000111001", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("030071", str);

    str = BasicFormat(value, 0, "O8");
    EXPECT_EQ("00030071", str);

    str = BasicFormat(value, 10, "O8");
    EXPECT_EQ("  00030071", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("3039", str);

    str = BasicFormat(value, 0, "X8");
    EXPECT_EQ("00003039", str);

    str = BasicFormat(value, 10, "X8");
    EXPECT_EQ("  00003039", str);
}

TEST(BasicFormatTest, FormatUInt16)
{
    uint16_t value = 12345;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("12345", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("12345     ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("     12345", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("12345", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("   12345", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("0011000000111001", str);

    // Binary string for 16 bit int is always 16 characters long
    str = BasicFormat(value, 0, "B23");
    EXPECT_EQ("0011000000111001", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("030071", str);

    str = BasicFormat(value, 0, "O8");
    EXPECT_EQ("00030071", str);

    str = BasicFormat(value, 10, "O8");
    EXPECT_EQ("  00030071", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("3039", str);

    str = BasicFormat(value, 0, "X8");
    EXPECT_EQ("00003039", str);

    str = BasicFormat(value, 10, "X8");
    EXPECT_EQ("  00003039", str);
}

TEST(BasicFormatTest, FormatInt32)
{
    int32_t value = 12345678;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("12345678  ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("  12345678", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("00000000101111000110000101001110", str);

    // Binary string for 32 bit int is always 32 characters long
    str = BasicFormat(value, 0, "B37");
    EXPECT_EQ("00000000101111000110000101001110", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("00057060516", str);

    str = BasicFormat(value, 0, "O9");
    EXPECT_EQ("057060516", str);

    str = BasicFormat(value, 10, "O9");
    EXPECT_EQ(" 057060516", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("00BC614E", str);

    str = BasicFormat(value, 0, "X6");
    EXPECT_EQ("BC614E", str);

    str = BasicFormat(value, 10, "X8");
    EXPECT_EQ("  00BC614E", str);
}

TEST(BasicFormatTest, FormatUInt32)
{
    uint32_t value = 12345678;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, -10, "");
    EXPECT_EQ("12345678  ", str);

    str = BasicFormat(value, 10, "");
    EXPECT_EQ("  12345678", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, 0, "D8");
    EXPECT_EQ("12345678", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("00000000101111000110000101001110", str);

    // Binary string for 32 bit int is always 32 characters long
    str = BasicFormat(value, 0, "B37");
    EXPECT_EQ("00000000101111000110000101001110", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("00057060516", str);

    str = BasicFormat(value, 0, "O9");
    EXPECT_EQ("057060516", str);

    str = BasicFormat(value, 10, "O9");
    EXPECT_EQ(" 057060516", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("00BC614E", str);

    str = BasicFormat(value, 0, "X6");
    EXPECT_EQ("BC614E", str);

    str = BasicFormat(value, 10, "X8");
    EXPECT_EQ("  00BC614E", str);
}

TEST(BasicFormatTest, FormatInt64)
{
    int64_t value = 1234567890;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("1234567890", str);

    str = BasicFormat(value, -18, "");
    EXPECT_EQ("1234567890        ", str);

    str = BasicFormat(value, 18, "");
    EXPECT_EQ("        1234567890", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("1234567890", str);

    str = BasicFormat(value, 0, "D12");
    EXPECT_EQ("  1234567890", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("0000000000000000000000000000000001001001100101100000001011010010", str);

    // Binary string for 64 bit int is always 64 characters long
    str = BasicFormat(value, 0, "B80");
    EXPECT_EQ("0000000000000000000000000000000001001001100101100000001011010010", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("0000000000011145401322", str);

    str = BasicFormat(value, 0, "O12");
    EXPECT_EQ("011145401322", str);

    str = BasicFormat(value, 18, "O12");
    EXPECT_EQ("      011145401322", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("00000000499602D2", str);

    str = BasicFormat(value, 0, "X8");
    EXPECT_EQ("499602D2", str);

    str = BasicFormat(value, 18, "X8");
    EXPECT_EQ("          499602D2", str);
}

TEST(BasicFormatTest, FormatUInt64)
{
    uint64_t value = 1234567890;

    std::string str = BasicFormat(value, 0, "");
    EXPECT_EQ("1234567890", str);

    str = BasicFormat(value, -18, "");
    EXPECT_EQ("1234567890        ", str);

    str = BasicFormat(value, 18, "");
    EXPECT_EQ("        1234567890", str);

    str = BasicFormat(value, 0, "D");
    EXPECT_EQ("1234567890", str);

    str = BasicFormat(value, 0, "D12");
    EXPECT_EQ("  1234567890", str);

    str = BasicFormat(value, 0, "B");
    EXPECT_EQ("0000000000000000000000000000000001001001100101100000001011010010", str);

    // Binary string for 64 bit int is always 64 characters long
    str = BasicFormat(value, 0, "B80");
    EXPECT_EQ("0000000000000000000000000000000001001001100101100000001011010010", str);

    str = BasicFormat(value, 0, "O");
    EXPECT_EQ("0000000000011145401322", str);

    str = BasicFormat(value, 0, "O12");
    EXPECT_EQ("011145401322", str);

    str = BasicFormat(value, 18, "O12");
    EXPECT_EQ("      011145401322", str);

    str = BasicFormat(value, 0, "X");
    EXPECT_EQ("00000000499602D2", str);

    str = BasicFormat(value, 0, "X8");
    EXPECT_EQ("499602D2", str);

    str = BasicFormat(value, 18, "X8");
    EXPECT_EQ("          499602D2", str);
}

TEST(FormatParameter, FormatEmpty)
{
    size_t parameterIndex {};
    std::string formatString = "{}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {0}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatIndex)
{
    size_t parameterIndex {};
    std::string formatString = "{1}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {1}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatInvalidIndex)
{
    size_t parameterIndex {};
    std::string formatString = "{2}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_FALSE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {2}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatWidth)
{
    size_t parameterIndex {};
    std::string formatString = "{,10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {0}, parameterIndex);
    EXPECT_EQ(int {10}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatIndexWidth)
{
    size_t parameterIndex {};
    std::string formatString = "{1,10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {1}, parameterIndex);
    EXPECT_EQ(int {10}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatIndexNegativeWidth)
{
    size_t parameterIndex {};
    std::string formatString = "{1,-10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {1}, parameterIndex);
    EXPECT_EQ(int {-10}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatInvalidIndexNegativeWidth)
{
    size_t parameterIndex {};
    std::string formatString = "{2,-10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_FALSE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {2}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string {}, formatSpecifier);
}

TEST(FormatParameter, FormatFormatSpecifier)
{
    size_t parameterIndex {};
    std::string formatString = "{:D10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {0}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string{"D10"}, formatSpecifier);
}

TEST(FormatParameter, FormatWidthFormatSpecifier)
{
    size_t parameterIndex {};
    std::string formatString = "{,10:D10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {0}, parameterIndex);
    EXPECT_EQ(int {10}, width);
    EXPECT_EQ(std::string{ "D10" }, formatSpecifier);
}

TEST(FormatParameter, FormatIndexFormatSpecifier)
{
    size_t parameterIndex {};
    std::string formatString = "{1:D10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {1}, parameterIndex);
    EXPECT_EQ(int {0}, width);
    EXPECT_EQ(std::string{ "D10" }, formatSpecifier);
}

TEST(FormatParameter, FormatIndexWidthFormatSpecifier)
{
    size_t parameterIndex {};
    std::string formatString = "{1,10:D10}";
    int width {};
    std::string formatSpecifier;
    const char* formatStringStart = formatString.data();
    const char* formatStringEnd = formatString.data() + formatString.length() - 1;

    EXPECT_TRUE(FormatParameter(parameterIndex, 2, formatStringStart, formatStringEnd, width, formatSpecifier));
    EXPECT_EQ(size_t {1}, parameterIndex);
    EXPECT_EQ(int {10}, width);
    EXPECT_EQ(std::string{ "D10" }, formatSpecifier);
}

TEST(FormatTest, FormatCharPtr)
{
    const std::string reference = "Hello";
    const char* text = "Hello";
    std::string str;
    Format(str, text);

    EXPECT_EQ(reference, str);
}

TEST(FormatTest, FormatString)
{
    const std::string reference = "Hello";
    std::string text = "Hello";
    std::string str;
    Format(str, text);

    EXPECT_EQ(reference, str);
}

TEST(FormatTest, FormatNoFormat)
{
    const std::string text = "";
    std::string str;
    Format(str, "", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Not all parameters have been used. Parameters not used: 0", GetFormatError());
}

TEST(FormatTest, FormatSimple)
{
    const std::string text = "123 456";
    std::string str;
    Format(str, "{} {}", 123, 456);

    EXPECT_EQ(std::string(text), str);
}

TEST(FormatTest, FormatIndexes)
{
    const std::string text = "123 456 World Hello";
    std::string str;
    Format(str, "{} {} {3} {2}", 123, 456, "Hello", "World");

    EXPECT_EQ(std::string(text), str);
}

TEST(FormatTest, FormatWithWidth)
{
    const std::string text = "       123 456           World Hello   ";
    std::string str;
    Format(str, "{,10} {,-10} {3,8} {2,-8}", 123, 456, "Hello", "World");

    EXPECT_EQ(std::string(text), str);
}

TEST(FormatTest, FormatIntegralWithFormat)
{
    const std::string text = " 305419896    305419896 00010010001101000101011001111000  02215053170   12345678";
    std::string str;
    int value = 0x12345678;
    Format(str, "{0:D10} {0,12:D10} {0,32:B32} {0,12:O11} {0,10:X8}", value);

    EXPECT_EQ(std::string(text), str);
}

TEST(FormatTest, FormatFloatingPointWithFormat)
{
    std::ostringstream refStream;
    std::string str;
    double value = 1234.5678;
    Format(str, "{0,10} {0,10:F4.4} {0,10:F3.4} {0,10:F3.3} {0,12:F5.5} {0,10:E4.4} {0,10:G5.4}", value);

    refStream << FormatStandard<double>(10, 0, 0, value);
    refStream << ' ';
    refStream << FormatFixed<double>(10, 4, 4, value);
    refStream << ' ';
    refStream << FormatFixed<double>(10, 3, 4, value);
    refStream << ' ';
    refStream << FormatFixed<double>(10, 3, 3, value);
    refStream << ' ';
    refStream << FormatFixed<double>(12, 5, 5, value);
    refStream << ' ';
    refStream << FormatScientific<double>(10, 4, 4, value);
    refStream << ' ';
    refStream << FormatStandard<double>(10, 5, 4, value);

    EXPECT_EQ(refStream.str(), str);
}

TEST(FormatTest, FormatInvalidIndex)
{
    const std::string text = "";
    std::string str;
    Format(str, "{a}", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Invalid index specified: a", GetFormatError());
}

TEST(FormatTest, FormatIndexOutOfRangeLow)
{
    const std::string text = "";
    std::string str;
    Format(str, "{-1}", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Invalid index specified: -1", GetFormatError());
}

TEST(FormatTest, FormatIndexOutOfRangeHigh)
{
    const std::string text = "";
    std::string str;
    Format(str, "{1}", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Invalid index specified: 1 should be between 0 and 0", GetFormatError());
}

TEST(FormatTest, FormatInvalidWidth)
{
    const std::string text = "";
    std::string str;
    Format(str, "{0,a}", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Invalid width specified: a", GetFormatError());
}

TEST(FormatTest, FormatTooFewParameters)
{
    const std::string text = "123 ";
    std::string str;
    Format(str, "{0} {1}", 123);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Invalid index specified: 1 should be between 0 and 0", GetFormatError());
}

TEST(FormatTest, FormatUnusedParameters)
{
    const std::string text = "123";
    std::string str;
    Format(str, "{0}", 123, 456);

    EXPECT_EQ(text, str);
    ASSERT_TRUE(HaveFormatError());
    EXPECT_EQ("Not all parameters have been used. Parameters not used: 1", GetFormatError());
}

namespace {

enum class MyType {
    Yes,
    No
};

class MyClass
{
public:
    int X, Y;
};

} // anonymous namespace

TEST(FormatTest, FormatEnumClass)
{
    const std::string expected = "Yes";
    std::string str;
    Format(str, "{0}", MyType::Yes);

    EXPECT_EQ(expected, str);
    ASSERT_FALSE(HaveFormatError());
}

TEST(FormatTest, FormatClass)
{
    const std::string expected = "(1,2)";
    std::string str;
    Format(str, "{0}", MyClass{ 1,2 });

    EXPECT_EQ(expected, str);
    ASSERT_FALSE(HaveFormatError());
}

} // namespace utility

namespace serialization {

template<>
const serialization::BidirectionalMap<utility::MyType, std::string> EnumSerializationMap<utility::MyType>::ConversionMap =
{
    {utility::MyType::Yes, "Yes"},
    {utility::MyType::No, "No"},
};

template<>
inline std::string Serialize(const utility::MyClass& value)
{
    return utility::FormatString("({0},{1})", value.X, value.Y);
}

} // namespace serialization

