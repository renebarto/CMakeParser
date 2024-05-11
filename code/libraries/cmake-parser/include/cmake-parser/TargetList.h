#pragma once

#include <map>
#include "cmake-parser/Target.h"

namespace cmake_parser {

using Targets = std::map<std::string, TargetPtr>;

class TargetList
{
private:
    Targets m_targets;

public:
    TargetList();
    TargetList(const TargetList& other);
    TargetList& operator = (const TargetList& other);

    const Targets& GetTargets() const { return m_targets; }
    TargetPtr FindTarget(const std::string& name) const;
    void AddTarget(const std::string& name, TargetPtr target);

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;
};

inline std::ostream& operator << (std::ostream& stream, const TargetList& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser