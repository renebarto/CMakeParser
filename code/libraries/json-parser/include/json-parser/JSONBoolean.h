#pragma once

#include "json-parser/JSONValue.h"

namespace json_parser {

class JSONBoolean : public JSONValue
{
public:
    JSONBoolean();
    JSONBoolean(bool value);
    virtual ~JSONBoolean() {};

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual JSONValueType Type() const override { return JSONValueType::Boolean; }
    bool GetValue() const { return m_value; }
    void SetValue(bool value) { m_value = value; }

private:
    bool m_value;
};

using JSONBooleanPtr = std::shared_ptr<JSONBoolean>;

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONBoolean& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
