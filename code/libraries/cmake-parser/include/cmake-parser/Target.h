#pragma once

#include <memory>
#include <string>
#include "serialization/EnumSerialization.h"
#include "cmake-parser/Serialization.h"

namespace cmake_parser {

enum class TargetAttribute
{
    None = 0,
    Win32Exe = 1,
    MacOSXBundle = 2,
    ExcludeFromAll = 4,
    Imported = 8,
    Global = 16,
    Alias = 32,
};

inline TargetAttribute operator |(TargetAttribute lhs, TargetAttribute rhs)
{
    return static_cast<TargetAttribute>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
inline bool operator &(TargetAttribute lhs, TargetAttribute rhs)
{
    return (static_cast<int>(lhs) & static_cast<int>(rhs)) != 0;
}

std::string SerializeTargetAttribute(const TargetAttribute& value);

inline std::ostream& operator << (std::ostream& stream, const TargetAttribute& value)
{
    return stream << SerializeTargetAttribute(value);
}

} // namespace cmake_parser

namespace serialization {

template<>
inline std::string Serialize(const cmake_parser::TargetAttribute& value)
{
    return cmake_parser::SerializeTargetAttribute(value);
}

} // namespace serialization

namespace cmake_parser {

class Target
{
private:
    std::string m_name;
    TargetAttribute m_attributes;
    std::string m_sources;
    std::string m_aliasTarget;

public:
    Target() = default;
    Target(const std::string& name, TargetAttribute attributes = TargetAttribute::None, const std::string& sources = {}, const std::string& aliasTarget = {});

    const std::string& Name() const { return m_name; }
    bool IsImported() const { return (m_attributes & TargetAttribute::Imported); }
    bool IsGlobal() const { return (m_attributes & TargetAttribute::Global); }
    bool IsAlias() const { return (m_attributes & TargetAttribute::Alias); }
    TargetAttribute Attributes() const { return m_attributes; }
    std::string Sources() const { return m_sources; }
    std::string AliasTarget() const { return IsAlias() ? m_aliasTarget : ""; }

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;
};
using TargetPtr = std::shared_ptr<Target>;

inline std::ostream& operator << (std::ostream& stream, const Target& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const TargetPtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser