#include "cmake-parser/Project.h"

#include <sstream>

using namespace cmake_parser;

Project::Project(const std::string& name)
    : m_name{ name }
    , m_version{}
    , m_description{}
    , m_languages{}
    , m_parentProject{}
{
}

Project::Project(const std::string& name, ProjectPtr parentProject)
    : m_name{ name }
    , m_version{}
    , m_description{}
    , m_languages{}
    , m_parentProject{ parentProject }
{
}

std::string Project::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    auto parent = m_parentProject.lock();
    std::string parentName;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        if (parent == nullptr)
        {
            parentName = "none";
        }
        else
        {
            parentName = parent->Name();
        }
        stream
            << "Project name = " << m_name
            << ", version = " << m_version
            << ", description = " << m_description
            << ", languages = " << m_languages
            << ", parent = " << parentName;
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"name\": " << SerializeString(format, Name()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"version\": " << SerializeString(format, Version()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"description\": " << SerializeString(format, Description()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"languages\": " << SerializeString(format, Languages()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"parent\": ";
        if (parent == nullptr)
        {
            stream << "null";
        }
        else
        {
            stream << SerializeString(format, parent->Name());
        }
        stream << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"targets\": " << std::endl;
        stream << m_targets.Serialize(format, indent + 8) << std::endl;
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

