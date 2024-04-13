#pragma once

#include <iostream>

#pragma warning(disable: 4826)
#pragma warning(disable: 4868)
#pragma warning(disable: 4355)
#include "gtest/gtest.h"
#pragma warning(default: 4826)
#pragma warning(default: 4868)
#pragma warning(default: 4355)

#define EXPECT_NULL(a) EXPECT_TRUE(nullptr == (a))
#define EXPECT_NOT_NULL(a) EXPECT_FALSE(nullptr == (a))
#define ASSERT_NULL(a) ASSERT_TRUE(nullptr == (a))
#define ASSERT_NOT_NULL(a) ASSERT_FALSE(nullptr == (a))
