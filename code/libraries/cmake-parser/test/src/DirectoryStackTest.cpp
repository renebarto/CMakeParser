#include "cmake-parser/DirectoryStack.h"

#include "test-platform/GoogleTest.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {

class DirectoryStackTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    DirectoryStackTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
        Tracing::SetDefaultTraceFilter(TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information/* | TraceCategory::Data | TraceCategory::Debug*/);
    }
    void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

TEST_F(DirectoryStackTest, Construct)
{
    DirectoryStack stack;

    EXPECT_EQ(size_t{ 0 }, stack.Size());
    EXPECT_NULL(stack.Top());
    EXPECT_NULL(stack.At(0));
}

TEST_F(DirectoryStackTest, Push)
{
    DirectoryStack stack;

    EXPECT_EQ(size_t{ 0 }, stack.Size());
    auto directory = std::make_shared<Directory>("x", "y");
    stack.Push(directory);
    EXPECT_EQ(size_t{ 1 }, stack.Size());
    EXPECT_EQ(directory, stack.Top());
    EXPECT_EQ(directory, stack.At(0));
    EXPECT_NULL(stack.At(1));
}

TEST_F(DirectoryStackTest, Pop)
{
    DirectoryStack stack;

    auto directory = std::make_shared<Directory>("x", "y");
    stack.Push(directory);
    EXPECT_EQ(size_t{ 1 }, stack.Size());
    EXPECT_EQ(directory, stack.Top());
    EXPECT_EQ(directory, stack.At(0));
    EXPECT_NULL(stack.At(1));

    EXPECT_EQ(directory, stack.Pop());
    EXPECT_EQ(size_t{ 0 }, stack.Size());
    EXPECT_NULL(stack.Top());
    EXPECT_NULL(stack.At(0));
}

TEST_F(DirectoryStackTest, PushPopMultiple)
{
    DirectoryStack stack;

    auto directory1 = std::make_shared<Directory>("a", "x");
    stack.Push(directory1);
    auto directory2 = std::make_shared<Directory>("b", "y", directory1);
    stack.Push(directory2);
    EXPECT_EQ(size_t{ 2 }, stack.Size());
    EXPECT_EQ(directory2, stack.Top());
    EXPECT_EQ(directory2, stack.At(0));
    EXPECT_EQ(directory1, stack.At(1));
    EXPECT_NULL(stack.At(2));

    EXPECT_EQ(directory2, stack.Pop());
    EXPECT_EQ(size_t{ 1 }, stack.Size());
    EXPECT_EQ(directory1, stack.Top());
    EXPECT_EQ(directory1, stack.At(0));
    EXPECT_NULL(stack.At(1));

    EXPECT_EQ(directory1, stack.Pop());
    EXPECT_EQ(size_t{ 0 }, stack.Size());
    EXPECT_NULL(stack.Top());
    EXPECT_NULL(stack.At(0));
}

} // namespace cmake_parser
