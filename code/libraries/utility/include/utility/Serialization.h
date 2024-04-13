#pragma once

#include <chrono>
#include <sstream>
#include <string>
#include <vector>
#include "serialization/Serialization.h"

namespace serialization {

// String serialization
// Every serialization specialization uses a width parameter, which is used for expansion
// width < 0 Left aligned
// width > 0 right aligned
// width < actual length no alignment
template<>
inline std::string Serialize(const bool& value)
{
    return std::string(value ? "true" : "false");
}

std::string Serialize(char value, int width = 0);
std::string Serialize(wchar_t value, int width = 0);
std::string Serialize(int8_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(uint8_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(int16_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(uint16_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(int32_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(uint32_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(int64_t value, int width = 0, int base = 10, bool showHexBase = false);
std::string Serialize(uint64_t value, int width = 0, int base = 10, bool showHexBase = false);
#if defined(PLATFORM_WINDOWS)
inline std::string Serialize(long unsigned int value, int width = 0, int base = 10, bool showHexBase = false)
{
    return Serialize(static_cast<uint32_t>(value), width, base, showHexBase);
}
#endif
enum class FloatingPointRepresentation
{
    Fixed,
    Exponential,
    Mixed
};
std::string Serialize(float value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
std::string Serialize(double value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
std::string Serialize(long double value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
std::string Serialize(const std::string& value, int width = 0, bool quote = false);
std::string Serialize(const char* value, int width = 0, bool quote = false);
std::string Serialize(const void* value, int width = 0);
std::string Serialize(void* value, int width = 0);
std::string SerializeData(const std::uint8_t* value, std::size_t size);
std::string SerializeData(const std::vector<std::uint8_t>& value);
std::string SerializeMACAddress(const std::vector<std::uint8_t>& value);
// Needed as osal cannot implement serialization as it cannot depend on utility

template<typename T>
std::string Serialize(const std::vector<T>& value)
{
    std::ostringstream stream;
    stream << "[";
    for (auto it = value.begin(); it != value.end(); ++it)
    {
        stream << *it;
        auto nextIt = it + 1;
        if (nextIt != value.end())
        {
            stream << ",";
        }
    }
    stream << "]";
    return stream.str();

}
template<typename T>
std::string Serialize(const std::vector<T>& value, int width)
{
    return serialization::Align(Serialize(value), width);
}

template<>
inline std::string Serialize<std::uint8_t>(const std::vector<std::uint8_t>& value)
{
    std::ostringstream stream;
    stream << "[";
    for (auto it = value.begin(); it != value.end(); ++it)
    {
        stream << Serialize(*it, 0, 16, true);
        auto nextIt = it + 1;
        if (nextIt != value.end())
        {
            stream << ",";
        }
    }
    stream << "]";
    return stream.str();
}

} // namespace serialization
