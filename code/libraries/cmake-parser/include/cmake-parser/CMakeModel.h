#pragma once

#include <filesystem>
#include "cmake-parser/CMakeCache.h"
#include "cmake-parser/DirectoryList.h"
#include "cmake-parser/ProjectList.h"
#include "cmake-parser/VariableList.h"

namespace cmake_parser {

enum class VariableAttribute
{
    None = 0,
    Cache = 1,
    ParentScope = 2,
    Force = 4,
};

inline VariableAttribute operator |(VariableAttribute lhs, VariableAttribute rhs)
{
    return static_cast<VariableAttribute>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
inline bool operator &(VariableAttribute lhs, VariableAttribute rhs)
{
    return (static_cast<int>(lhs) & static_cast<int>(rhs)) != 0;
}

class CMakeModel
{
private:
    CMakeCache m_cache;
    VariableList* m_scopeVariables;
    VariableList m_environment;
    ProjectList m_projects;
    ProjectPtr m_currentProject;
    DirectoryList m_directories;
    DirectoryPtr m_currentDirectory;
    bool m_isSourceRootSet;

public:
    CMakeModel();

    bool IsSourceRootSet() const { return m_isSourceRootSet; }
    void SetupSourceRoot(const std::filesystem::path& root);
    void SetupRootCMakeFile(const std::filesystem::path& rootListFile);
    void SetupCMakePath(const std::filesystem::path& cmakePath, const std::string& cmakeVersion);
    void SetupNinjaPath(const std::filesystem::path& ninjaPath);

    const TypedVariables& GetCacheVariables() const { return m_cache.GetVariables(); }
    const Variables& GetVariables() const;
    const Variables& GetEnvironmentVariables() const { return m_environment.GetVariables(); }
    std::string GetCacheVariable(const std::string& name) const { return m_cache.GetVariable(name); }
    TypedVariablePtr FindCacheVariable(const std::string& name) const { return m_cache.FindVariable(name); }
    std::string GetVariable(const std::string& name) const;
    std::string GetEnvironmentVariable(const std::string& name) const { return m_environment.GetVariable(name); }
    VariablePtr FindVariable(const std::string& name) const;
    VariablePtr FindEnvironmentVariable(const std::string& name) const { return m_environment.FindVariable(name); }
    void SetVariable(const std::string& name, const std::string& value, VariableAttribute attributes = {}, const std::string& type = {});
    void SetEnvironmentVariable(const std::string& name, const std::string& value);
    void UnsetVariable(const std::string& name, VariableAttribute attributes = {});
    void UnsetEnvironmentVariable(const std::string& name);

    const Projects& GetProjects() const { return m_projects.GetProjects(); }
    const ProjectPtr GetProject(const std::string& name) const { return m_projects.GetProject(name); }
    bool AddProject(ProjectPtr project);
    Projects GetSubProjects(ProjectPtr parentProject) { return m_projects.GetSubProjects(parentProject); }
    const ProjectPtr GetParentProject(ProjectPtr project) const { return m_projects.GetParentProject(project); }
    const ProjectPtr GetMainProject() const { return m_projects.GetMainProject(); }
    
    const Directories& GetDirectories() const { return m_directories.GetDirectories(); }
    std::filesystem::path GetDirectory(const std::filesystem::path& path) const { return m_directories.GetDirectory(path); }
    DirectoryPtr FindDirectory(const std::filesystem::path& path) const { return m_directories.FindDirectory(path); }
    bool AddDirectory(DirectoryPtr directory) { return m_directories.AddDirectory(directory); }

    Directories GetSubDirectories(DirectoryPtr parentDirectory) { return m_directories.GetSubDirectories(parentDirectory); }
    DirectoryPtr GetParentDirectory(DirectoryPtr directory) const { return m_directories.GetParentDirectory(directory); }
    DirectoryPtr GetRootDirectory() const { return m_directories.GetRootDirectory(); }
    DirectoryPtr GetCurrentDirectory() const;

    void AddMessage(const std::string& messageMode, const std::string& message);
};

} // namespace cmake_parser