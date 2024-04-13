#pragma once

#include <sstream>
#include <string>
#include "serialization/Deserialization.h"

namespace serialization {

// String Deserialization
bool Deserialize(const std::string& text, bool& value);
bool Deserialize(const std::string& text, int8_t& value, int base = 10);
bool Deserialize(const std::string& text, uint8_t& value, int base = 10);
bool Deserialize(const std::string& text, int16_t& value, int base = 10);
bool Deserialize(const std::string& text, uint16_t& value, int base = 10);
bool Deserialize(const std::string& text, int32_t& value, int base = 10);
bool Deserialize(const std::string& text, uint32_t& value, int base = 10);
bool Deserialize(const std::string& text, int64_t& value, int base = 10);
bool Deserialize(const std::string& text, uint64_t& value, int base = 10);
bool Deserialize(const std::string& text, float& value);
bool Deserialize(const std::string& text, double& value);

bool Deserialize(const std::string& text, std::string& value);
bool Deserialize(const std::string& text, std::wstring& value);

} // namespace serialization
