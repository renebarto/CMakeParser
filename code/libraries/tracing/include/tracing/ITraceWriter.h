#pragma once

#include "tracing/TraceCategory.h"

namespace tracing {

class ITraceWriter
{
public:
    virtual ~ITraceWriter() {}

    virtual void Trace(
        TraceCategory category,
        const std::string& fileName, 
        int line, 
        const std::string& functionName, 
        const std::string& msg) = 0;
};

} // namespace tracing
