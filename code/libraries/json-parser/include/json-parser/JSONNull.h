#pragma once

#include "json-parser/JSONValue.h"

namespace json_parser {

class JSONNull : public JSONValue
{
public:
    JSONNull();
    virtual ~JSONNull() = default;

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual JSONValueType Type() const override { return JSONValueType::Null; }

private:
};

} // namepsace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONNull& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
