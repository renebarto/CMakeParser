#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include "json-parser/json.h"

namespace json_parser {

inline JSONValuePtr CreateNull()
{
    return std::make_shared<JSONNull>();
}
JSONValuePtr CreateNumber(const std::string& value);
JSONValuePtr CreateString(const std::string& value);
template<class T>
inline JSONValuePtr CreateArray(const std::vector<T>& values)
{
    auto result = std::make_shared<JSONArray>();
    for (auto const& value : values)
    {
        result->AddValue(CreateValue(value));
    }
    return result;
}
JSONValuePtr CreateArray();
JSONValuePtr CreateArray(const std::vector<JSONValuePtr>& values);

JSONValuePtr CreateObject();


template <class T>
inline JSONValuePtr CreateValue(const T& value)
{
    return nullptr;
}
template <>
inline JSONValuePtr CreateValue(const std::string& value)
{
    return std::make_shared<JSONString>(value);
}
JSONValuePtr CreateValue(const char* value);
template <>
inline JSONValuePtr CreateValue<bool>(const bool& value)
{
    return std::make_shared<JSONBoolean>(value);
}
template <>
inline JSONValuePtr CreateValue<nullptr_t>(const nullptr_t& /*value*/)
{
    return CreateNull();
}
template <>
inline JSONValuePtr CreateValue(const int8_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const uint8_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const int16_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const uint16_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const int32_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const uint32_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const int64_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const uint64_t& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const float& value)
{
    return std::make_shared<JSONNumber>(value);
}
template <>
inline JSONValuePtr CreateValue(const double& value)
{
    return std::make_shared<JSONNumber>(value);
}

} // namespace json_parser
