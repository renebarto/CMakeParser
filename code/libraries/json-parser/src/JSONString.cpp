#include "json-parser/JSONString.h"
#include "utility/Deserialization.h"
#include "utility/Serialization.h"
#include "utility/StringFunctions.h"

namespace json_parser
{

static constexpr size_t IndentSize = 4;

JSONString::JSONString()
    : m_value()
{
}

JSONString::JSONString(const std::string & value)
    : m_value(utility::UnQuote(value))
{
}

JSONString::JSONString(const char* value)
    : m_value(utility::UnQuote(value))
{
}

void JSONString::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '"' << m_value << '"';
}

} // namespace json_parser
