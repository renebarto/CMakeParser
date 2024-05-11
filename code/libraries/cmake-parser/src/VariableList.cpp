#include "cmake-parser/VariableList.h"

#include <sstream>

using namespace cmake_parser;

VariableList::VariableList()
    : m_variables{}
{
}

VariableList::VariableList(const VariableList& other)
    : m_variables{}
{
    for (auto const& var : other.m_variables)
    {
        if (var.second != nullptr)
        {
            SetVariable(var.first, var.second->Value());
        }
    }
}

VariableList& VariableList::operator = (const VariableList& other)
{
    if (this != &other)
    {
        m_variables.clear();
        for (auto const& var : other.m_variables)
        {
            if (var.second != nullptr)
            {
                SetVariable(var.first, var.second->Value());
            }
        }
    }
    return *this;
}

std::string VariableList::GetVariable(const std::string& name) const
{
    auto var = FindVariable(name);
    if (!var)
        return {};
    return var->Value();
}

void VariableList::SetVariable(const std::string& name, const std::string& value)
{
    AddVariable(name, std::make_shared<Variable>(name, value));
}

void VariableList::UnsetVariable(const std::string& name)
{
    if (FindVariable(name) != nullptr)
    {
        m_variables.erase(name);
    }
}

VariablePtr VariableList::FindVariable(const std::string& name) const
{
    auto it = m_variables.find(name);
    return (it == m_variables.end()) ? nullptr : it->second;
}

void VariableList::AddVariable(const std::string& name, VariablePtr variable)
{
    m_variables.insert(std::pair(name, variable));
}

std::string VariableList::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "VariableList:" << std::endl;
        for (auto const& var : m_variables)
        {
            if (var.second != nullptr)
                stream << std::string(indent, ' ') << var.second->Serialize() << std::endl;
        }
        break;
    case SerializationFormat::JSON:
        {
            stream << std::string(indent, ' ') << "[";
            bool first = true;
            for (auto const& var : m_variables)
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
                if (var.second != nullptr)
                    stream << var.second->Serialize(format, indent + 4);
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

