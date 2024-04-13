#pragma once

#include "json-parser/JSONValue.h"
#include "json-parser/JSONArray.h"
#include "json-parser/JSONBoolean.h"
#include "json-parser/JSONNull.h"
#include "json-parser/JSONNumber.h"
#include "json-parser/JSONObject.h"
#include "json-parser/JSONString.h"
//#include "json-parser/Create.h"

namespace json_parser {

inline bool IsArray(JSONValuePtr value)
{
    return value->Type() == JSONValueType::Array;
}

inline bool IsObject(JSONValuePtr value)
{
    return value->Type() == JSONValueType::Object;
}

inline bool IsBoolean(JSONValuePtr value)
{
    return value->Type() == JSONValueType::Boolean;
}

inline bool IsNumber(JSONValuePtr value)
{
    return value->Type() == JSONValueType::Number;
}

inline bool IsString(JSONValuePtr value)
{
    return value->Type() == JSONValueType::String;
}

inline bool IsNull(JSONValuePtr value)
{
    return value->Type() == JSONValueType::Null;
}

inline std::shared_ptr<JSONArray> ToArray(JSONValuePtr value)
{
    return std::dynamic_pointer_cast<JSONArray>(value);
}

//inline std::shared_ptr<Object> ToObject(JSONValuePtr value)
//{
//    return std::dynamic_pointer_cast<Object>(value);
//}

inline std::shared_ptr<JSONBoolean> ToBoolean(JSONValuePtr value)
{
    return std::dynamic_pointer_cast<JSONBoolean>(value);
}

inline std::shared_ptr<JSONNumber> ToNumber(JSONValuePtr value)
{
    return std::dynamic_pointer_cast<JSONNumber>(value);
}

inline std::shared_ptr<JSONString> ToString(JSONValuePtr value)
{
    return std::dynamic_pointer_cast<JSONString>(value);
}

inline std::shared_ptr<JSONNull> ToNull(JSONValuePtr value)
{
    return std::dynamic_pointer_cast<JSONNull>(value);
}

} // namespace json_parser
