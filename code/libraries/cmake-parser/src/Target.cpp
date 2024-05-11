#include "cmake-parser/Target.h"

#include <sstream>
#include "utility/Serialization.h"

using namespace cmake_parser;

std::string cmake_parser::SerializeTargetAttribute(const TargetAttribute& value)
{
    if (value == TargetAttribute::None)
        return "None";
    std::string result;
    if (value & TargetAttribute::Win32Exe)
    {
        result += "Win32Exe";
    }
    if (value & TargetAttribute::MacOSXBundle)
    {
        if (!result.empty())
            result += "|";
        result += "MacOSXBundle";
    }
    if (value & TargetAttribute::ExcludeFromAll)
    {
        if (!result.empty())
            result += "|";
        result += "ExcludeFromAll";
    }
    if (value & TargetAttribute::Imported)
    {
        if (!result.empty())
            result += "|";
        result += "Imported";
    }
    if (value & TargetAttribute::Global)
    {
        if (!result.empty())
            result += "|";
        result += "Global";
    }
    if (value & TargetAttribute::Alias)
    {
        if (!result.empty())
            result += "|";
        result += "Alias";
    }
    return result;
}

Target::Target(const std::string& name, TargetAttribute attributes/* = TargetAttribute::None*/, const std::string& sources /*= {}*/, const std::string& aliasTarget /*= {}*/)
    : m_name{ name }
    , m_attributes{ attributes }
    , m_sources{ sources }
    , m_aliasTarget{ aliasTarget }
{

}

std::string Target::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "Target " << Name() << " [" << Sources() << "]" << " Attributes " << Attributes() << (IsAlias() ? " " + AliasTarget() : "");
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"name\": " << SerializeString(format, Name()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"attributes\": \"" << Attributes() << "\"," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"sources\": " << SerializeString(format, Sources());
        if (IsAlias())
        {
            stream << "," << std::endl;
            stream << std::string(indent + 4, ' ') << "\"aliasTarget\": " << SerializeString(format, AliasTarget()) << std::endl;
        }
        else
        {
            stream << std::endl;
        }
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

