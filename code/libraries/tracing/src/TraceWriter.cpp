#include "tracing/TraceWriter.h"

#include <sstream>

#include "utility/Serialization.h"

namespace tracing {

TraceWriter::TraceWriter(ITraceLineWriter& writer)
    : m_writer(writer)
{
}

void TraceWriter::Trace(
    TraceCategory category,
    const std::string& fileName, 
    int line, 
    const std::string& functionName, 
    const std::string& msg)
{
    std::ostringstream stream;
    stream << category << "|" << fileName << ":" << line << "|" << functionName << "|" << msg;
    m_writer.WriteLine(category, stream.str());
}

} // namespace tracing
