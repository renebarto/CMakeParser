#pragma once

#include <map>
#include "cmake-parser/TypedVariable.h"

namespace cmake_parser {

using TypedVariables = std::map<std::string, TypedVariablePtr>;

class TypedVariableList
{
private:
    TypedVariables m_variables;

public:
    TypedVariableList();
    TypedVariableList(const TypedVariableList& other);
    TypedVariableList& operator = (const TypedVariableList& other);

    const TypedVariables& GetVariables() const { return m_variables; }
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& type, const std::string& value, const std::string& description);
    void UnsetVariable(const std::string& name);

    TypedVariablePtr FindVariable(const std::string& name) const;
    void AddVariable(const std::string& name, TypedVariablePtr variable);

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;
};

inline std::ostream& operator << (std::ostream& stream, const TypedVariableList& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser