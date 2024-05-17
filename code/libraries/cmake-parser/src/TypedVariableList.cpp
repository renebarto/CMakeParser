#include "cmake-parser/TypedVariableList.h"

#include <sstream>

using namespace cmake_parser;

TypedVariableList::TypedVariableList()
    : m_variables{}
{
}

TypedVariableList::TypedVariableList(const TypedVariableList& other)
    : m_variables{}
{
    for (auto const& var : other.m_variables)
    {
        if (var.second != nullptr)
        {
            SetVariable(var.first, var.second->Type(), var.second->Value(), var.second->Description());
        }
    }
}

TypedVariableList& TypedVariableList::operator = (const TypedVariableList& other)
{
    if (this != &other)
    {
        m_variables.clear();
        for (auto const& var : other.m_variables)
        {
            if (var.second != nullptr)
            {
                SetVariable(var.first, var.second->Type(), var.second->Value(), var.second->Description());
            }
        }
    }
    return *this;
}

std::string TypedVariableList::GetVariable(const std::string& name) const
{
    auto var = FindVariable(name);
    if (!var)
        return {};
    return var->Value();
}

void TypedVariableList::SetVariable(const std::string& name, const std::string& type, const std::string& value, const std::string& description)
{
    AddVariable(name, std::make_shared<TypedVariable>(name, type, value, description));
}

void TypedVariableList::UnsetVariable(const std::string& name)
{
    if (FindVariable(name) != nullptr)
    {
        m_variables.erase(name);
    }
}

TypedVariablePtr TypedVariableList::FindVariable(const std::string& name) const
{
    auto it = m_variables.find(name);
    return (it == m_variables.end()) ? nullptr : it->second;
}

void TypedVariableList::AddVariable(const std::string& name, TypedVariablePtr variable)
{
    m_variables.insert(std::pair(name, variable));
}

std::string TypedVariableList::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "TypedVariableList:" << std::endl;
        for (auto const& var : m_variables)
        {
            if (var.second != nullptr)
                stream << var.second->Serialize() << std::endl;
        }
        break;
    case SerializationFormat::JSON:
        stream << SerializeList(format, indent, *this);
        break;
    default:
        break;
    }
    return stream.str();
}

