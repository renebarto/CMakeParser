#include "test-platform/GoogleTest.h"

#include "serialization/EnumSerialization.h"

namespace {

enum class TestBool
{
    True,
    False
};

} // namespace anonymous

namespace serialization {

template<>
const serialization::BidirectionalMap<TestBool, std::string> EnumSerializationMap<TestBool>::ConversionMap =
{
    { TestBool::True, "True"},
    { TestBool::False, "False"},
};

} // namespace serialization

namespace utility {

TEST(EnumSerializationTest, SerializeTestBool)
{
    EXPECT_EQ("True", serialization::Serialize(TestBool::True));
    EXPECT_EQ("False", serialization::Serialize(TestBool::False));
}

TEST(EnumSerializationTest, SerializeTestBoolWidth)
{
    EXPECT_EQ("True  ", serialization::Serialize(TestBool::True, -6));
    EXPECT_EQ(" False", serialization::Serialize(TestBool::False, 6));
}

TEST(EnumSerializationTest, SerializeTestBoolDefault)
{
    EXPECT_EQ("XYZ", serialization::Serialize(static_cast<TestBool>(2), "XYZ"));
    EXPECT_EQ("  XYZ", serialization::Serialize(static_cast<TestBool>(2), "XYZ", 5));
    EXPECT_EQ("XYZ  ", serialization::Serialize(static_cast<TestBool>(2), "XYZ", -5));
}

TEST(EnumSerializationTest, DeserializeTestBool)
{
    TestBool value {};
    EXPECT_TRUE(serialization::Deserialize("True", value));
    EXPECT_EQ(TestBool::True, value);
    EXPECT_TRUE(serialization::Deserialize("False", value));
    EXPECT_EQ(TestBool::False, value);
}

TEST(EnumSerializationTest, DeserializeTestBoolInvalid)
{
    TestBool value{};
    EXPECT_FALSE(serialization::Deserialize("Junk", value));
    EXPECT_EQ(TestBool{}, value);
}

} // namespace utility
