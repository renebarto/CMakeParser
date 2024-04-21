#pragma once

#include <map>
#include "cmake-parser/Variable.h"

namespace cmake_parser {

using Variables = std::map<std::string, VariablePtr>;

class VariableList
{
private:
    Variables m_variables;

public:
    VariableList();
    VariableList(const VariableList& other);
    VariableList& operator = (const VariableList& other);

    const Variables& GetVariables() const { return m_variables; }
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value);
    void UnsetVariable(const std::string& name);

    VariablePtr FindVariable(const std::string& name) const;
    void AddVariable(const std::string& name, VariablePtr variable);

    std::string Serialize() const;
};

inline std::ostream& operator << (std::ostream& stream, const VariableList& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser