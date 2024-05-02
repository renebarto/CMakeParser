#pragma once

#include <sstream>
#include <string>
#include "serialization/EnumSerialization.h"

namespace serialization {

template<typename T>
typename std::enable_if<!std::is_enum<T>::value, std::string>::type
Serialize(const T& t)
{
    std::ostringstream stream;
    stream << t;
    return stream.str();
}

template<typename T>
typename std::enable_if<!std::is_enum<T>::value, std::string>::type
Serialize(const T& t, int width)
{
    return Align(Serialize(t), width);
}

} // namespace serialization
