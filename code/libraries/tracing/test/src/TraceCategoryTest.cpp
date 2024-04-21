#include "test-platform/GoogleTest.h"

#include "tracing/TraceCategory.h"

#include "serialization/Serialization.h"

namespace tracing {

TEST(TraceCategoryTest, Serialize)
{
    EXPECT_EQ("Error", serialization::Serialize(TraceCategory::Error));
    EXPECT_EQ("Warn ", serialization::Serialize(TraceCategory::Warning));
    EXPECT_EQ("Info ", serialization::Serialize(TraceCategory::Information));
    EXPECT_EQ("Data ", serialization::Serialize(TraceCategory::Data));
    EXPECT_EQ("Debug", serialization::Serialize(TraceCategory::Debug));
}

TEST(TraceCategoryTest, EnumTraceCategories)
{
    std::vector<TraceCategory> expected = {
        TraceCategory::Error,
        TraceCategory::Warning,
        TraceCategory::Information,
        TraceCategory::Data,
        TraceCategory::Debug,
    };
    EXPECT_EQ(expected, EnumTraceCategories());
}

} // namespace tracing
