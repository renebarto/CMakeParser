#include "json-parser/json.h"

namespace json_parser
{

static constexpr size_t IndentSize = 4;

JSONKVPair::JSONKVPair()
    : m_key()
    , m_value()
{
}

JSONKVPair::JSONKVPair(const std::string& key, const JSONValuePtr& value)
    : m_key(key)
    , m_value(value)
{
}

void JSONKVPair::Serialize(std::ostream& stream, int indentDepth) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << std::string(IndentSize, ' ');
    }
    stream << "\"" << m_key << "\" : ";
    if (m_value != nullptr)
    {
        m_value->Serialize(stream, indentDepth, false);
    }
    else
        stream << "null";
}

JSONObject::JSONObject()
    : m_elements()
{

}

JSONObject::JSONObject(const std::initializer_list<JSONKVPair>& elements)
    : m_elements()
{
    for (auto& element : elements)
    {
        AddPair(element);
    }
}

bool JSONObject::AddPair(const JSONKVPair& pair)
{
    auto result = m_elements.insert(std::pair<const std::string&, JSONValuePtr>(pair.GetKey(), pair.GetValue()));
    return result.second;
}

const JSONValuePtr JSONObject::Find(const std::string& name) const
{
    auto it = m_elements.find(name);
    if (it != m_elements.end())
    {
        return it->second;
    }
    return nullptr;
}

JSONValuePtr JSONObject::Find(const std::string& name)
{
    auto it = m_elements.find(name);
    if (it != m_elements.end())
    {
        return it->second;
    }
    return nullptr;
}

const JSONBooleanPtr JSONObject::GetBoolean(const std::string& name) const
{
    auto value = Find(name);
    if (value == nullptr)
        return nullptr;
    if (!value->IsBoolean())
        return nullptr;
    return value->AsBoolean();
}

const JSONNumberPtr JSONObject::GetNumber(const std::string& name) const
{
    auto value = Find(name);
    if (value == nullptr)
        return nullptr;
    if (!value->IsNumber())
        return nullptr;
    return value->AsNumber();
}

const JSONStringPtr JSONObject::GetString(const std::string& name) const
{
    auto value = Find(name);
    if (value == nullptr)
        return nullptr;
    if (!value->IsString())
        return nullptr;
    return value->AsString();
}

const JSONArrayPtr JSONObject::GetArray(const std::string& name) const
{
    auto value = Find(name);
    if (value == nullptr)
        return nullptr;
    if (!value->IsArray())
        return nullptr;
    return value->AsArray();
}

const JSONObjectPtr JSONObject::GetObject(const std::string& name) const
{
    auto value = Find(name);
    if (value == nullptr)
        return nullptr;
    if (!value->IsObject())
        return nullptr;
    return value->AsObject();
}


void JSONObject::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '{' << std::endl;
    ConstIterator iterator = GetIterator();
    while (!iterator.AtEnd())
    {
        (*iterator).Serialize(stream, indentDepth + 1);
        iterator++;
        if (!iterator.AtEnd())
            stream << ',';
        stream << std::endl;
    }
    for (int i = 0; i < indentDepth; i++)
    {
        stream << std::string(IndentSize, ' ');
    }
    stream << '}';
}

} // namespace json_parser
