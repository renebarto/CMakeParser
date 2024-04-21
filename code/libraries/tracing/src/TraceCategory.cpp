#include "tracing/TraceCategory.h"

#include "serialization/EnumSerialization.h"

namespace serialization {

template<>
const BidirectionalMap<tracing::TraceCategory, std::string> EnumSerializationMap<tracing::TraceCategory>::ConversionMap = {
    { tracing::TraceCategory::Error, "Error"},
    { tracing::TraceCategory::Warning, "Warn "},
    { tracing::TraceCategory::Information, "Info "},
    { tracing::TraceCategory::Data, "Data "},
    { tracing::TraceCategory::Debug, "Debug"},
};

} // namespace serialization

namespace tracing {

std::ostream& operator << (std::ostream& stream, const TraceCategory& value)
{
    return stream << serialization::Serialize(value, "????");
}

std::vector<TraceCategory> EnumTraceCategories()
{
    return serialization::EnumSerializationMap<TraceCategory>::GetKeys();
}

} // namespace tracing
