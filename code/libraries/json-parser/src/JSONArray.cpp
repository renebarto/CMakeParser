#include "json-parser/JSONArray.h"
#include "json-parser/json.h"

namespace json_parser
{

static constexpr size_t IndentSize = 4;

JSONArray::JSONArray()
    : m_elements()
{
}

JSONArray::JSONArray(const std::initializer_list<JSONValuePtr>& elements)
    : m_elements()
{
    for (auto& element : elements)
    {
        AddValue(element);
    }
}

void JSONArray::AddValue(JSONValuePtr value)
{
    m_elements.push_back(value);
}

void JSONArray::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '[' << std::endl;
    ConstIterator iterator = GetIterator();
    while (!iterator.AtEnd())
    {
        (*iterator)->Serialize(stream, indentDepth + 1);
        iterator++;
        if (!iterator.AtEnd())
            stream << ',';
        stream << std::endl;
    }
    for (int i = 0; i < indentDepth; i++)
    {
        stream << std::string(IndentSize, ' ');
    }
    stream << ']';
}

} // namespace json_parser
