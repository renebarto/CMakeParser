#pragma once

#include <string>
#include "cmake-parser/VariableList.h"

namespace cmake_parser {

class CMakeCache
{
private:
    VariableList m_variables;

public:
    CMakeCache();

    const Variables& GetVariables() const;
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value);
    void UnsetVariable(const std::string& name);

    VariablePtr FindVariable(const std::string& name) const;
    void AddVariable(const std::string& name, VariablePtr variable);

    std::string Serialize() const;
};

inline std::ostream& operator << (std::ostream& stream, const CMakeCache& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser