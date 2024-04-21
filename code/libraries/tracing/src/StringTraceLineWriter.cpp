#include "tracing/StringTraceLineWriter.h"

namespace tracing {

void StringTraceLineWriter::WriteLine(TraceCategory /*category*/, const std::string& line)
{
    m_result.push_back(line);
}

void StringTraceLineWriter::ResetResult()
{
    m_result = {};
}

std::vector<std::string> StringTraceLineWriter::GetResult() const
{
    return m_result;
}

} // namespace tracing
