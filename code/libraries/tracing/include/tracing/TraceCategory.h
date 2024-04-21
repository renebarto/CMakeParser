#pragma once

#include <functional>
#include <string>
#include <vector>

namespace tracing {

enum class TraceCategory : uint32_t
{
    Error       = 0x00000001u,
    Warning     = 0x00000002u,
    Information = 0x00000004u,
    Data        = 0x00000008u,
    Debug       = 0x00000010u,
    None        = 0x00000000u,
    All         = 0xFFFFFFFFu,
};

std::ostream& operator << (std::ostream& stream, const TraceCategory& value);

std::vector<TraceCategory> EnumTraceCategories();

} // namespace tracing
