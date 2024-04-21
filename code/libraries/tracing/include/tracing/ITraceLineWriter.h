#pragma once

#include <string>
#include "tracing/TraceCategory.h"

namespace tracing {

class ITraceLineWriter {
public:
    virtual ~ITraceLineWriter() = default;

    virtual void WriteLine(TraceCategory category, const std::string& line) = 0;
};

} // namespace tracing 
