#include "tracing/ConsoleTraceLineWriter.h"

#include "utility/Console.h"

static utility::Console s_console;

namespace tracing {

static utility::ConsoleColor GetColorForCategory(TraceCategory category)
{
    switch (category)
    {
    case TraceCategory::Error:          return utility::ConsoleColor::Red;
    case TraceCategory::Warning:        return utility::ConsoleColor::Magenta | utility::ConsoleColor::Intensity;
    case TraceCategory::Information:    return utility::ConsoleColor::Green;
    case TraceCategory::Data:           return utility::ConsoleColor::Cyan;
    case TraceCategory::Debug:          return utility::ConsoleColor::Yellow;

    default: return utility::ConsoleColor::Default;
    }
}

void ConsoleTraceLineWriter::WriteLine(TraceCategory category, const std::string& line)
{
    s_console << fgcolor(GetColorForCategory(category));
    s_console << line << std::endl;
    s_console << fgcolor(utility::ConsoleColor::Default);
}

} // namespace tracing
