#pragma once

#include "json-parser/JSONValue.h"

namespace json_parser {

class JSONString : public JSONValue
{
public:
    JSONString();
    explicit JSONString(const std::string& value);
    JSONString(const char* value);
    virtual ~JSONString() {};

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    void Clear() { m_value.clear(); }
    virtual JSONValueType Type() const override { return JSONValueType::String; }
    const std::string& GetValue() const { return m_value; }
    void SetValue(const std::string& value) { m_value = value; }

private:
    std::string m_value;
};

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONString& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
