#include "json-parser/JSONValue.h"

#include <memory>
#include "json-parser/json.h"
#include "json-parser/Lexer.h"

namespace serialization {

template<>
const BidirectionalMap<json_parser::JSONValueType, std::string> EnumSerializationMap<json_parser::JSONValueType>::ConversionMap =
{
    { json_parser::JSONValueType::Null, "Null" },
    { json_parser::JSONValueType::Boolean, "Boolean" },
    { json_parser::JSONValueType::String, "String" },
    { json_parser::JSONValueType::Number, "Number" },
    { json_parser::JSONValueType::Object, "Object" },
    { json_parser::JSONValueType::Array, "Array" },
};

} // namespace serialization

namespace json_parser {

JSONArrayPtr JSONValue::AsArray()
{
    if (JSONValueType::Array == Type())
    {
        return std::dynamic_pointer_cast<JSONArray>(shared_from_this());
    }
    return nullptr;
}

JSONObjectPtr JSONValue::AsObject()
{
    if (JSONValueType::Object == Type())
    {
        return std::dynamic_pointer_cast<JSONObject>(shared_from_this());
    }
    return nullptr;
}

JSONNullPtr JSONValue::AsNull()
{
    if (JSONValueType::Null == Type())
    {
        return std::dynamic_pointer_cast<JSONNull>(shared_from_this());
    }
    return nullptr;
}

JSONBooleanPtr JSONValue::AsBoolean()
{
    if (JSONValueType::Boolean == Type())
    {
        return std::dynamic_pointer_cast<JSONBoolean>(shared_from_this());
    }
    return nullptr;
}

JSONNumberPtr JSONValue::AsNumber()
{
    if (JSONValueType::Number == Type())
    {
        return std::dynamic_pointer_cast<JSONNumber>(shared_from_this());
    }
    return nullptr;
}

JSONStringPtr JSONValue::AsString()
{
    if (JSONValueType::String == Type())
    {
        return std::dynamic_pointer_cast<JSONString>(shared_from_this());
    }
    return nullptr;
}

bool JSONValue::IsArray() const
{
    return Type() == JSONValueType::Array;
}

bool JSONValue::IsObject() const
{
    return Type() == JSONValueType::Object;
}

bool JSONValue::IsNull() const
{
    return Type() == JSONValueType::Null;
}

bool JSONValue::IsBoolean() const
{
    return Type() == JSONValueType::Boolean;
}

bool JSONValue::IsNumber() const
{
    return Type() == JSONValueType::Number;
}

bool JSONValue::IsString() const
{
    return Type() == JSONValueType::String;
}

} // namespace json_parser
