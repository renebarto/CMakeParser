#include "cmake-parser/Variable.h"

#include <sstream>

using namespace cmake_parser;

std::string Variable::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream  << "Variable " << Name() << " = " << Value();
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"name\": " << SerializeString(format, Name()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"value\": " << SerializeString(format, Value()) << std::endl;
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

