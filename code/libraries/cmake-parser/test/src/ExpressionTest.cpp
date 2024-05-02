#include "cmake-parser/Expression.h"

#include "test-platform/GoogleTest.h"
#include "utility/CommandLine.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {
namespace expression {

static std::filesystem::path FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return std::filesystem::path(stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0))).remove_filename();
    return {};
}

static std::string GetCMakeVersion()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("cmake --version", "", stdoutText, stderrText) == 0)
    {
        auto line = stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
        std::string prefix = "cmake version ";
        return line.substr(prefix.length());
    }
    return {};
}

static std::filesystem::path FindNinja()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

class ExpressionTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;
    CMakeModel model;

    ExpressionTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
        Tracing::SetDefaultTraceFilter(TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information/* | TraceCategory::Data | TraceCategory::Debug */);
        model.SetupSourceRoot(TEST_DATA_DIR, "cmake-x64-Debug");
        model.SetupCMakePath(FindCMake(), GetCMakeVersion());
        model.SetupNinjaPath(FindNinja());
        model.SetVariable("A", "varA");
        model.SetVariable("PLATFORM_NAME", "Windows");
    }
    void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

TEST_F(ExpressionTest, ParseString)
{
    std::string expr = "\"text\"";
    Expression expression(model, expr);

    EXPECT_EQ("\"text\"", expression.Evaluate());
}

TEST_F(ExpressionTest, ParseStringWithVariable)
{
    std::string expr = "\"text ${A}\"";
    Expression expression(model, expr);

    EXPECT_EQ("\"text varA\"", expression.Evaluate());
}

TEST_F(ExpressionTest, ParseExpressionWithVariables)
{
    std::string expr = "${CMAKE_SOURCE_DIR}/output/${PLATFORM_NAME}";
    Expression expression(model, expr);

    EXPECT_EQ(std::filesystem::path(TEST_DATA_DIR) / "output" / "Windows", expression.Evaluate());
}

} // namespace expression
} // namespace cmake_parser
