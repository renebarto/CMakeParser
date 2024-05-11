#pragma once

#include <sstream>

namespace cmake_parser {

enum class SerializationFormat
{
	Text,
	JSON,
};

inline std::string SerializeString(SerializationFormat format, const std::string& str)
{
	std::ostringstream stream;
	switch (format)
	{
	case SerializationFormat::Text:
		stream << str;
	case SerializationFormat::JSON:
		if (str.empty())
			return "null";
		stream << "\"" << str << "\"";
		break;
	default:
		break;
	}
	return stream.str();
}

} // cmake_parser
