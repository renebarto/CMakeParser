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

std::string Project::Serialize() const
{
    std::ostringstream stream;
    auto parent = m_parentProject.lock();
    std::string parentName;
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
    return stream.str();
}

