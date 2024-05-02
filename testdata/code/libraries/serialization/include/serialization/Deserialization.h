#pragma once

#include <sstream>
#include <string>

namespace serialization {

template<class T>
typename std::enable_if<!std::is_enum<T>::value, bool>::type
Deserialize(const std::string& text, T & value)
{
    T t{};
    IStringStreamType stream(text);
    stream >> t;
    if (!stream.bad())
    {
        value = t;
        return true;
    }
    return false;
}

} // namespace serialization
