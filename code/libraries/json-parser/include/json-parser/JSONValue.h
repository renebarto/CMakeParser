#pragma once

#include <iostream>
#include <memory>
#include "serialization/EnumSerialization.h"
#include "utility/Serialization.h"
#include "parser/Tokenizer.h"

namespace json_parser {

enum class JSONValueType
{
    Null,
    Boolean,
    String,
    Number,
    Object,
    Array,
};

inline std::ostream& operator << (std::ostream& stream, const JSONValueType& value)
{
    return stream << serialization::Serialize(value);
}

class JSONArray;
using JSONArrayPtr = std::shared_ptr<JSONArray>;
class JSONObject;
using JSONObjectPtr = std::shared_ptr<JSONObject>;
class JSONNull;
using JSONNullPtr = std::shared_ptr<JSONNull>;
class JSONBoolean;
using JSONBooleanPtr = std::shared_ptr<JSONBoolean>;
class JSONNumber;
using JSONNumberPtr = std::shared_ptr<JSONNumber>;
class JSONString;
using JSONStringPtr = std::shared_ptr<JSONString>;

class JSONValue
    : public std::enable_shared_from_this<JSONValue>
{
public:
    JSONValue() {}
    virtual ~JSONValue() {};

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const = 0;

    virtual JSONValueType Type() const = 0;
    JSONArrayPtr AsArray();
    JSONObjectPtr AsObject();
    JSONNullPtr AsNull();
    JSONBooleanPtr AsBoolean();
    JSONNumberPtr AsNumber();
    JSONStringPtr AsString();
    bool IsArray() const;
    bool IsObject() const;
    bool IsNull() const;
    bool IsBoolean() const;
    bool IsNumber() const;
    bool IsString() const;
};
using JSONValuePtr = std::shared_ptr<JSONValue>;
class Lexer;

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONValuePtr& value)
{
    std::ostringstream stream;
    value->Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization

namespace json_parser {
    
inline std::ostream& operator << (std::ostream& stream, const JSONValue& value)
{
    return stream << serialization::Serialize(value);
}

} // namespace json_parser
