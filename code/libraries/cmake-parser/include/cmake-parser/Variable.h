#pragma once

#include <ostream>
#include <memory>
#include <string>

namespace cmake_parser {

struct Variable
{
    std::string m_name;
    std::string m_value;

    Variable(const std::string& name)
        : m_name{ name }
        , m_value{}
    {
    }
    Variable(const std::string& name, const std::string& value)
        : m_name{ name }
        , m_value{ value }
    {
    }
    const std::string& Name() const { return m_name; }
    const std::string& Value() const { return m_value; }
    void SetValue(const std::string& value) { m_value = value; }

    std::string Serialize() const;
};
using VariablePtr = std::shared_ptr<Variable>;

inline std::ostream& operator << (std::ostream& stream, const Variable& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const VariablePtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser