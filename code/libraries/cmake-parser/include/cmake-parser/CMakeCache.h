#pragma once

#include <string>
#include "cmake-parser/TypedVariableList.h"

namespace cmake_parser {

class CMakeCache
{
private:
    TypedVariableList m_variables;

public:
    CMakeCache();

    const TypedVariables& GetVariables() const;
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& type, const std::string& value, const std::string& description);
    void UnsetVariable(const std::string& name);

    TypedVariablePtr FindVariable(const std::string& name) const;
    void AddVariable(const std::string& name, TypedVariablePtr variable);

    std::string Serialize() const;
};

inline std::ostream& operator << (std::ostream& stream, const CMakeCache& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser