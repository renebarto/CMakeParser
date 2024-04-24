#pragma once

#include <ostream>
#include <memory>
#include <string>

namespace cmake_parser {

struct TypedVariable
{
    std::string m_name;
    std::string m_type;
    std::string m_value;

    TypedVariable(const std::string& name)
        : m_name{ name }
        , m_type{}
        , m_value{}
    {
    }
    TypedVariable(const std::string& name, const std::string& type, const std::string& value)
        : m_name{ name }
        , m_type{ type }
        , m_value{ value }
    {
    }
    const std::string& Name() const { return m_name; }
    const std::string& Type() const { return m_type; }
    const std::string& Value() const { return m_value; }
    void SetValue(const std::string& value) { m_value = value; }
    void SetValue(const std::string& type, const std::string& value) { m_type = type; m_value = value; }

    std::string Serialize() const;
};
using TypedVariablePtr = std::shared_ptr<TypedVariable>;

inline std::ostream& operator << (std::ostream& stream, const TypedVariable& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const TypedVariablePtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser