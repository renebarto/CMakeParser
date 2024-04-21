#include "cmake-parser/CMakeModel.h"

#include <sstream>

using namespace cmake_parser;

CMakeCache::CMakeCache()
    : m_variables{}
{
}

const Variables& CMakeCache::GetVariables() const
{
    return m_variables.GetVariables();
}

std::string CMakeCache::GetVariable(const std::string& name) const
{
    return m_variables.GetVariable(name);
}

void CMakeCache::SetVariable(const std::string& name, const std::string& value)
{
    m_variables.SetVariable(name, value);
}

void CMakeCache::UnsetVariable(const std::string& name)
{
    m_variables.UnsetVariable(name);
}

VariablePtr CMakeCache::FindVariable(const std::string& name) const
{
    return m_variables.FindVariable(name);
}

void CMakeCache::AddVariable(const std::string& name, VariablePtr variable)
{
    m_variables.AddVariable(name, variable);
}

std::string CMakeCache::Serialize() const
{
    std::ostringstream stream;
    stream << "CMakeCache:" << std::endl;
    for (auto const& var : m_variables.GetVariables())
    {
        if (var.second != nullptr)
            stream << var.second->Serialize() << std::endl;
    }
    return stream.str();
}

