#include "json-parser/JSONBoolean.h"
#include "utility/Deserialization.h"
#include "utility/Serialization.h"

namespace json_parser
{

static constexpr size_t IndentSize = 4;

JSONBoolean::JSONBoolean()
    : m_value()
{
}

JSONBoolean::JSONBoolean(bool value)
    : m_value(value)
{
}

void JSONBoolean::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << (m_value ? "true" : "false");
}

} // namespace json_parser
