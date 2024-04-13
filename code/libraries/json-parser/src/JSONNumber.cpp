#include "json-parser/JSONNumber.h"
#include "utility/Deserialization.h"
#include "utility/Serialization.h"

namespace json_parser
{

static constexpr size_t IndentSize = 4;
static constexpr int NumDigitsFloat = 8;

JSONNumber::JSONNumber()
    : m_value()
{
}

JSONNumber::JSONNumber(const std::string& value)
    : m_value(value)
{
}

JSONNumber::JSONNumber(int8_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(uint8_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(int16_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(uint16_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(int32_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(uint32_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(int64_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(uint64_t value)
    : m_value(serialization::Serialize(value))
{
}

JSONNumber::JSONNumber(float value)
    : m_value(serialization::Serialize(value, 0, NumDigitsFloat, serialization::FloatingPointRepresentation::Mixed))
{
}

JSONNumber::JSONNumber(double value)
    : m_value(serialization::Serialize(value))
{
}

bool JSONNumber::GetValue(int8_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(uint8_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(int16_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(uint16_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(int32_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(uint32_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(int64_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(uint64_t& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(float& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

bool JSONNumber::GetValue(double& value) const
{
    value = {};
    return serialization::Deserialize(m_value, value);
}

void JSONNumber::SetValue(int8_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(uint8_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(int16_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(uint16_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(int32_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(uint32_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(int64_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(uint64_t value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::SetValue(float value)
{
    m_value = serialization::Serialize(value, 0, NumDigitsFloat, serialization::FloatingPointRepresentation::Mixed);
}

void JSONNumber::SetValue(double value)
{
    m_value = serialization::Serialize(value);
}

void JSONNumber::Serialize(std::ostream& stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << m_value;
}

} // namespace json_parser
