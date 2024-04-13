#include "utility/Deserialization.h"

#include <algorithm>
#include <climits>
#include <cctype>
#include <cfloat>
#include <cstring>
#include <sstream>
#include "utility/StringFunctions.h"

namespace serialization {

static bool HasValidCharactersForBase(const std::string& text, int base, bool maybeSigned)
{
	if (text.empty()) // TICS !PRE-CFL#013
		return false;

	switch (base)
	{
	case 2:
		for (size_t i = 0; i < text.length(); i++)
		{
			if (std::strchr("01", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
				return false;
		}
		break;
	case 8:
		for (size_t i = 0; i < text.length(); i++)
		{
			if (std::strchr("01234567", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
				return false;
		}
		break;
	case 10:
		for (size_t i = 0; i < text.length(); i++)
		{
			if (maybeSigned)
			{
				if (std::strchr("0123456789+-", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
					return false;
			}
			else
			{
				if (std::strchr("0123456789+", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
					return false;
			}
		}
		break;
	case 16:
		for (size_t i = 0; i < text.length(); i++)
		{
			if (std::strchr("0123456789ABCDEF", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
				return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

static bool HasValidCharactersFloatingPoint(const std::string& text)
{
	if (text.empty()) // TICS !PRE-CFL#013
		return false;
	for (size_t i = 0; i < text.length(); i++)
	{
		if (std::strchr("0123456789.+-Ee", static_cast<char>(std::toupper(text[i]))) == nullptr) // TICS !PRE-CFL#013
			return false;
	}
	return true;
}

bool Deserialize(const std::string& text, bool& value)
{
	if (utility::IsEqualIgnoreCase("true", text))
	{
		value = true;
		return true;
	}
	if (utility::IsEqualIgnoreCase("false", text))
	{
		value = false;
		return true;
	}

	return false;
}

bool Deserialize(const std::string& text, int8_t& value, int base)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, true)) // TICS !PRE-CFL#013
		return false;

	long result = strtol(text.c_str(), nullptr, base);
	if ((base == 10) && ((result < SCHAR_MIN) || (result > SCHAR_MAX))) // TICS !PRE-CFL#013
		return false;
	value = static_cast<int8_t>(result);

	return true;
}

bool Deserialize(const std::string& text, uint8_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, false)) // TICS !PRE-CFL#013
		return false;

	long result = strtol(text.c_str(), nullptr, base);
	if ((base == 10) && ((result < 0) || (result > UCHAR_MAX))) // TICS !PRE-CFL#013
		return false;
	value = static_cast<uint8_t>(result);

	return true;
}

bool Deserialize(const std::string& text, int16_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, true)) // TICS !PRE-CFL#013
		return false;

	long result = strtol(text.c_str(), nullptr, base);
	if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX))) // TICS !PRE-CFL#013
		return false;
	value = static_cast<int16_t>(result);

	return true;
}

bool Deserialize(const std::string& text, uint16_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, false)) // TICS !PRE-CFL#013
		return false;

	long result = strtol(text.c_str(), nullptr, base);
	if ((base == 10) && ((result < 0) || (result > USHRT_MAX))) // TICS !PRE-CFL#013
		return false;
	value = static_cast<uint16_t>(result);

	return true;
}

bool Deserialize(const std::string& text, int32_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, true)) // TICS !PRE-CFL#013
		return false;

	long result = strtol(text.c_str(), nullptr, base);
	unsigned long result_ul = strtoul(text.c_str(), nullptr, base);
	if (((result == LONG_MIN) || (result == LONG_MAX)) && (errno == ERANGE)) // TICS !PRE-CFL#013
		return false;
	if ((base == 10) && ((result < INT_MIN) || (result > INT_MAX))) // TICS !PRE-CFL#013
		return false;
	if ((base != 10) && (result_ul > INT_MAX))
	{
		value = static_cast<int32_t>(result_ul);
	}
	else
	{
		value = static_cast<int32_t>(result);
	}

	return true;
}

bool Deserialize(const std::string& text, uint32_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, false)) // TICS !PRE-CFL#013
		return false;

	unsigned long long result = strtoull(text.c_str(), nullptr, base);
	if ((base == 10) && (result > UINT_MAX)) // TICS !PRE-CFL#013
		return false;
	value = static_cast<uint32_t>(result);

	return true;
}

bool Deserialize(const std::string& text, int64_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, true)) // TICS !PRE-CFL#013
		return false;

	// For some strange reason stroull behaves incorrectly in some cases.
	long long result = strtoll(text.c_str(), nullptr, base);
	unsigned long long result_ull = strtoull(text.c_str(), nullptr, base);
	if (((result == LLONG_MIN) || (result == LLONG_MAX)) && (errno == ERANGE)) // TICS !PRE-CFL#013
		return false;
	if ((base == 10) && (result >= 0) && (result_ull > LLONG_MAX)) // TICS !PRE-CFL#013
		return false;
	if ((base != 10) && (result_ull > LLONG_MAX))
	{
		value = static_cast<int64_t>(result_ull);
	}
	else
	{
		value = result;
	}

	return true;
}

bool Deserialize(const std::string& text, uint64_t& value, int base /*= 10*/)
{
	value = {};
	if (!HasValidCharactersForBase(text, base, false)) // TICS !PRE-CFL#013
		return false;

	unsigned long long result = strtoull(text.c_str(), nullptr, base);
	value = static_cast<uint64_t>(result);

	return true;
}

bool Deserialize(const std::string& text, float& value)
{
	value = {};
	if (!HasValidCharactersFloatingPoint(text)) // TICS !PRE-CFL#013
		return false;

	double tmp{};
	std::istringstream stream(text);
	stream >> tmp;
	if ((std::abs(tmp) < FLT_MIN) || (std::abs(tmp) > FLT_MAX)) // TICS !PRE-CFL#013
		return false;

	value = static_cast<float>(tmp);
	return true;
}

bool Deserialize(const std::string& text, double& value)
{
	value = {};
	if (!HasValidCharactersFloatingPoint(text)) // TICS !PRE-CFL#013
		return false;

	std::istringstream stream(text);
	stream >> value;

	return true;
}

bool Deserialize(const std::string& text, std::string& value)
{
	value = text;
	return true;
}

} // namespace serialization
