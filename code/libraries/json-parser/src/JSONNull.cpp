#include "json-parser/JSONNull.h"
#include "utility/Deserialization.h"
#include "utility/Serialization.h"

namespace json_parser {

static constexpr size_t IndentSize = 4;

JSONNull::JSONNull()
{
}

void JSONNull::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << "null";
}

} // namespace json_parser
