#include "test-platform/GoogleTest.h"

#include <regex>
#include "MockTraceWriter.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/StringTraceLineWriter.h"
#include "tracing/TraceWriter.h"
#include "tracing/Tracing.h"

namespace tracing {

static void CheckMatch(const std::vector<std::string>& regexStrings, const std::vector<std::string>& actual)
{
    EXPECT_EQ(regexStrings.size(), actual.size());
    for (std::size_t i = 0; i < regexStrings.size(); ++i)
    {
        std::regex regex(regexStrings[i]);
        std::smatch match;
        EXPECT_TRUE(std::regex_match(actual[i], match, regex));
    }
}

class TracingTest : public ::testing::Test
{
public:
    int m_exitCode;
    CategorySet<TraceCategory> m_savedTraceFilter;
    StringTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    TracingTest()
        : m_exitCode()
        , m_savedTraceFilter {}
        , m_lineWriter {}
        , m_traceWriter(m_lineWriter)
    {}

    virtual void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
    }
    virtual void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

TEST_F(TracingTest, IfTracingFunctionsSetButNothingEnabledNothingHappens)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::None);
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    CheckMatch({ }, m_lineWriter.GetResult());
}

TEST_F(TracingTest, IfTracingFunctionsSetAndCategoryEnabledTraceIsWritten)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TracingOnlyForEnabledCategory)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::Information);
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, IfNoTracingEnableFunctionSetOnlyDefaultCategoriesAreEnabled)
{
    // Assumption: default only Information and Data enabled
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Debug, "MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, IfDefaultFilterIsChangedOnlySpecifiedCategoriesAreEnabled)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::Information);
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Data, "MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TracingCategories)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    Tracing::Trace(TraceCategory::Error, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Warning, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Data, "MyFile", 123, "MyFunction", "Hello World");
    Tracing::Trace(TraceCategory::Debug, "MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Error\\|MyFile\\:123\\|MyFunction\\|Hello World",
        "Warn \\|MyFile\\:123\\|MyFunction\\|Hello World",
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World",
        "Data \\|MyFile\\:123\\|MyFunction\\|Hello World",
        "Debug\\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TracingWithFormat)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::Information);
    Tracing::Trace(TraceCategory::Information, "MyFile", 123, "MyFunction", "{0} {1} (C {2,4:D})", "Hello", "World", 2020);
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World \\(C 2020\\)"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceErrorString)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceError("MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Error\\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceErrorFormatted)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceError("MyFile", 123, "MyFunction", "{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Error\\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceWarningString)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceWarning("MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Warn \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceWarningFormatted)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceWarning("MyFile", 123, "MyFunction", "{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Warn \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceInfoString)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceInfo("MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceInfoFormatted)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceInfo("MyFile", 123, "MyFunction", "{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Info \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDataString)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceData("MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Data \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDataFormatted)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceData("MyFile", 123, "MyFunction", "{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Data \\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDebugString)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceDebug("MyFile", 123, "MyFunction", "Hello World");
    std::vector<std::string> regexStrings{
        "Debug\\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDebugFormatted)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceDebug("MyFile", 123, "MyFunction", "{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Debug\\|MyFile\\:123\\|MyFunction\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::Information);
    TRACE_INFRA(TraceCategory::Information, "{0} {1} (C {2,4:D})", "Hello", "World", 2020);
    std::vector<std::string> regexStrings{
        "Info \\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World \\(C 2020\\)"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceErrorMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TRACE_ERROR("{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Error\\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceWarningMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TRACE_WARNING("{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Warn \\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceInfoMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TRACE_INFO("{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Info \\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDataMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TRACE_DATA("{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Data \\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceDebugMacro)
{
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TRACE_DEBUG("{0} {1}", "Hello", "World");
    std::vector<std::string> regexStrings{
        "Debug\\|\\w+.cpp\\:\\d+\\|\\w+\\|Hello World"
    };
    CheckMatch(regexStrings, m_lineWriter.GetResult());
}

TEST_F(TracingTest, TraceToConsole)
{
    ConsoleTraceLineWriter lineWriter;
    TraceWriter writer(lineWriter);
    Tracing::SetTraceWriter(&writer);
    Tracing::SetDefaultTraceFilter(TraceCategory::All);
    TraceError("MyFile", 123, "MyFunction", "Error");
    TraceWarning("MyFile", 123, "MyFunction", "Warning");
    TraceInfo("MyFile", 123, "MyFunction", "Info");
    TraceData("MyFile", 123, "MyFunction", "Data");
    TraceDebug("MyFile", 123, "MyFunction", "Debug");
}

} // namespace tracing
