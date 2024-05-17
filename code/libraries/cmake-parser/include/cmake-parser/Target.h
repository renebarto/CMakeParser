#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include "serialization/EnumSerialization.h"
#include "cmake-parser/List.h"
#include "cmake-parser/Serialization.h"
#include "cmake-parser/VariableList.h"

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
    Static = 64,
    Shared = 128,
    Module = 256,
    Object = 512,
    Interface = 1024,
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

enum class TargetType
{
    None,
    Executable,
    StaticLibrary,
    SharedLibrary,
    Object,
};

extern const std::string TargetPropertyOutputName;
extern const std::string TargetPropertyIncludeDirectories;
extern const std::string TargetPropertyInterfaceIncludeDirectories;
extern const std::string TargetPropertyCompileDefinitions;
extern const std::string TargetPropertyInterfaceCompileDefinitions;
extern const std::string TargetPropertyCompileOptions;
extern const std::string TargetPropertyInterfaceCompileOptions;

extern std::string ExecutableTargetName(const std::string& targetName);
extern std::string StaticLibraryTargetName(const std::string& targetName);
extern std::string SharedLibraryTargetName(const std::string& targetName);
extern std::string ObjectLibraryTargetName(const std::string& targetName);

class BuildEntry
{
private:
    std::filesystem::path m_rootPath;
    std::filesystem::path m_binaryPath;
    std::filesystem::path m_sourcePath;
    std::string m_compileDefinitions;
    std::string m_compileOptions;
    std::string m_interfaceCompileDefinitions;
    std::string m_interfaceCompileOptions;

public:
    BuildEntry(const std::filesystem::path& rootDirectory, const std::filesystem::path& binaryDirectory, const std::string& sourcePath);
    
    void SetCompileDefinitions(const std::string& compileDefinitions) { m_compileDefinitions = compileDefinitions; }
    void SetInterfaceCompileDefinitions(const std::string& interfaceCompileDefinitions) { m_interfaceCompileDefinitions = interfaceCompileDefinitions; }
    void SetCompileOptions(const std::string& compileOptions) { m_compileOptions = compileOptions; }
    void SetInterfaceCompileOptions(const std::string& interfaceCompileOptions) { m_interfaceCompileOptions = interfaceCompileOptions; }

    std::filesystem::path SourcePath() const { return m_sourcePath; }
    std::string CompileDefinitions() const { return m_compileDefinitions; }
    std::string InterfaceCompileDefinitions() const { return m_interfaceCompileDefinitions; }
    std::string CompileOptions() const { return m_compileOptions; }
    std::string InterfaceCompileOptions() const { return m_interfaceCompileOptions; }
};

using BuildEntries = std::vector<BuildEntry>;

class Project;
using ProjectPtr = std::shared_ptr<Project>;
class Directory;
using DirectoryPtr = std::shared_ptr<Directory>;

class Target
{
private:
    std::string m_name;
    TargetType m_type;
    TargetAttribute m_attributes;
    List m_sources;
    std::string m_aliasTarget;
    VariableList m_properties;
    std::weak_ptr<Project> m_project;

public:
    Target() = delete;
    Target(ProjectPtr project);
    Target(ProjectPtr project, const std::string& name, TargetType type, TargetAttribute attributes = TargetAttribute::None, const std::string& sources = {}, const std::string& aliasTarget = {});

    const std::string& Name() const { return m_name; }
    TargetType Type() const { return m_type; }
    bool IsImported() const { return (m_attributes & TargetAttribute::Imported); }
    bool IsGlobal() const { return (m_attributes & TargetAttribute::Global); }
    bool IsAlias() const { return (m_attributes & TargetAttribute::Alias); }
    const Variables& GetProperties() const { return m_properties.GetVariables(); }
    std::string GetProperty(const std::string& name) const { return m_properties.GetVariable(name); }
    void SetProperty(const std::string& name, const std::string& value);
    void UnsetProperty(const std::string& name);
    const Variables& GetVariables() const;
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value);
    void UnsetVariable(const std::string& name);
    TargetAttribute Attributes() const { return m_attributes; }
    const List& Sources() const { return m_sources; }
    std::string CompileDefinitions() const;
    std::string InterfaceCompileDefinitions() const;
    std::string CompileOptions() const;
    std::string InterfaceCompileOptions() const;
    ProjectPtr Project() const { return m_project.lock(); }

    BuildEntries GetBuildEntries() const;
    std::string AliasTarget() const { return IsAlias() ? m_aliasTarget : ""; }

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;

private:
    DirectoryPtr GetDirectory() const;
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