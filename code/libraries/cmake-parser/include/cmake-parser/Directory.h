#pragma once

#include <filesystem>
#include "cmake-parser/VariableList.h"

namespace cmake_parser {

class Directory
{
    std::filesystem::path m_sourcePath;
    std::filesystem::path m_binaryPath;
    VariableList m_variables;
    std::weak_ptr<Directory> m_parentDirectory;

public:
    Directory(const std::filesystem::path& sourcePath, const std::filesystem::path& binaryPath);
    Directory(const std::filesystem::path& sourcePath, const std::filesystem::path& binaryPath, std::shared_ptr<Directory> parent);
    ~Directory();
    const std::filesystem::path& SourcePath() const { return m_sourcePath; }
    const std::filesystem::path& BinaryPath() const { return m_binaryPath; }
    std::shared_ptr<Directory> Parent() { return m_parentDirectory.lock(); }
    const Variables& GetVariables() const { return m_variables.GetVariables(); }
    std::string GetVariable(const std::string& name) const { return m_variables.GetVariable(name); }
    void SetVariable(const std::string& name, const std::string& value) { m_variables.SetVariable(name, value); }
    void UnsetVariable(const std::string& name) { m_variables.UnsetVariable(name); }

    std::string SerializeShort() const;
    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;

    const VariableList& GetVariableList() const { return m_variables; }
    VariableList& GetVariableList() { return m_variables; }
};
using DirectoryPtr = std::shared_ptr<Directory>;
using WeakDirectoryPtr = std::weak_ptr<Directory>;

inline std::ostream& operator << (std::ostream& stream, const Directory& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const DirectoryPtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser
