#include "cmake-parser/TypedVariable.h"

#include <sstream>

using namespace cmake_parser;

std::string TypedVariable::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "TypedVariable " << Name() << ":" << Type() << " = " << Value() << " (" << Description() << ")";
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"name\": " << SerializeString(format, Name()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"type\": " << SerializeString(format, Type()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"value\": " << SerializeString(format, Value()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"description\": " << SerializeString(format, Description()) << std::endl;
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

