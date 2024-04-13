//------------------------------------------------------------------------------
// Copyright   : Copyright(c) 2021 René Barto
//
// File        : TypeCastTest.cpp
//
// Namespace   : utility
//
// Class       : TypeCastTest
//
// Description :
//
//------------------------------------------------------------------------------

#include "test-platform/GoogleTest.h"

#include "utility/TypeCast.h"

namespace utility {

TEST(TypeCastTest, CastToInteger8bits) {
    std::int8_t value = 123;
    EXPECT_EQ(std::uint8_t{ 123 }, CastToInteger(value));
}

TEST(TypeCastTest, CastToInteger16bits) {
    std::int16_t value = 12345;
    EXPECT_EQ(std::uint16_t{ 12345 }, CastToInteger(value));
}

TEST(TypeCastTest, CastToInteger32bits) {
    std::int32_t value = 12345678;
    EXPECT_EQ(std::uint32_t{ 12345678 }, CastToInteger(value));
}

TEST(TypeCastTest, CastToInteger64bits) {
    std::int64_t value = 1234567890123;
    EXPECT_EQ(std::uint64_t{ 1234567890123 }, CastToInteger(value));
}

#if defined(PLATFORM_WINDOWS)
#pragma warning(disable: 4127)
#endif
TEST(TypeCastTest, CastPointerToInteger) {
    if (sizeof(void*) == 4)
    {
        void* p = reinterpret_cast<void*>(std::int32_t{ 12345678 });
        EXPECT_EQ(std::uint32_t{ 12345678 }, CastPointerToInteger(p));
    }
    else
    {
        void* p = reinterpret_cast<void*>(std::int32_t{ 12345678 });
        EXPECT_EQ(std::uint64_t{ 12345678 }, CastPointerToInteger(p));
    }
}
#if defined(PLATFORM_WINDOWS)
#pragma warning(default: 4127)
#endif

} // namespace utility
