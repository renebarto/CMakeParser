#include "cmake-parser/CMakeModel.h"

#include <sstream>

using namespace cmake_parser;

#define VAR(name, type, value, description) { #name, {#type, value, description }}

static const std::map<std::string, std::tuple<std::string, std::string, std::string>> DefaultVariables
{
//    VAR(CMAKE_BUILD_TYPE, STRING, "Debug"),
};

CMakeCache::CMakeCache()
    : m_variables{}
{
    for (auto var : DefaultVariables)
    {
        auto variable = std::make_shared<TypedVariable>(var.first, std::get<0>(var.second), std::get<1>(var.second), std::get<2>(var.second));
        AddVariable(var.first, variable);
    }
}

const TypedVariables& CMakeCache::GetVariables() const
{
    return m_variables.GetVariables();
}

std::string CMakeCache::GetVariable(const std::string& name) const
{
    return m_variables.GetVariable(name);
}

void CMakeCache::SetVariable(const std::string& name, const std::string& type, const std::string& value, const std::string& description)
{
    m_variables.SetVariable(name, type, value, description);
}

void CMakeCache::UnsetVariable(const std::string& name)
{
    m_variables.UnsetVariable(name);
}

TypedVariablePtr CMakeCache::FindVariable(const std::string& name) const
{
    return m_variables.FindVariable(name);
}

void CMakeCache::AddVariable(const std::string& name, TypedVariablePtr variable)
{
    m_variables.AddVariable(name, variable);
}

std::string CMakeCache::Serialize(SerializationFormat format /*= SerializationFormat::Text*/, unsigned indent /*= 0*/) const
{
    std::ostringstream stream;
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "CMakeCache:" << std::endl;
        for (auto const& var : m_variables.GetVariables())
        {
            if (var.second != nullptr)
                stream << var.second->Serialize() << std::endl;
        }
        break;
    case SerializationFormat::JSON:
        {
            stream << std::string(indent, ' ') << "[";
            bool first = true;
            for (auto const& dir : m_variables.GetVariables())
            {
                if (first)
                {
                    stream << std::endl;
                }
                else
                {
                    stream << "," << std::endl;
                }
                first = false;
                if (dir.second != nullptr)
                    stream << dir.second->Serialize(format, indent + 4);
                else
                    stream << "null";
            }
            if (!first)
            {
                stream << std::endl << std::string(indent, ' ');
            }
            stream << "]";
        }
        break;
    default:
        break;
    }
    return stream.str();
}

