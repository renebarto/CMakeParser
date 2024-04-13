#include "json-parser/JSONFactory.h"

#include "utility/StringFunctions.h"

namespace json_parser {

JSONValuePtr CreateValue(const char* value)
{
    return std::make_shared<JSONString>(value);
}
JSONValuePtr CreateArray()
{
    return std::make_shared<JSONArray>();
}
JSONValuePtr CreateArray(const std::vector<JSONValuePtr>& values)
{
    auto result = std::make_shared<JSONArray>();
    for (auto const& value : values)
    {
        result->AddValue(value);
    }
    return result;
}

JSONValuePtr CreateObject()
{
    return std::make_shared<JSONObject>();
}
JSONValuePtr CreateNumber(const std::string& value)
{
    return std::make_shared<JSONNumber>(value);
}
JSONValuePtr CreateString(const std::string& value)
{
    return std::make_shared<JSONString>(utility::UnQuote(value));
}

} // namespace json_parser
