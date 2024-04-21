#pragma once

#include <filesystem>
#include "cmake-parser/CMakeCache.h"
#include "cmake-parser/DirectoryList.h"
#include "cmake-parser/ProjectList.h"
#include "cmake-parser/VariableList.h"

namespace cmake_parser {

enum VariableAttribute
{
    Cache = 1,
    ParentScope = 2,
    Force = 4,
};

class CMakeModel
{
private:
    CMakeCache m_cache;
    VariableList* m_scopeVariables;
    ProjectList m_projects;
    DirectoryList m_directories;
    bool m_isSourceRootSet;

public:
    CMakeModel();

    bool IsSourceRootSet() const { return m_isSourceRootSet; }
    void SetupSourceRoot(const std::filesystem::path& root);
    void SetupRootCMakeFile(const std::string& rootListFile);
    void SetupCMakePath(const std::string& cmakePath, const std::string& cmakeVersion);
    void SetupNinjaPath(const std::string& ninjaPath);

    const Variables& GetCacheVariables() const { return m_cache.GetVariables(); }
    const Variables& GetVariables() const;
    std::string GetCacheVariable(const std::string& name) const { return m_cache.GetVariable(name); }
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value, VariableAttribute attributes = {});
    void UnsetVariable(const std::string& name, VariableAttribute attributes = {});

    const Projects& GetProjects() const { return m_projects.GetProjects(); }
    const ProjectPtr GetProject(const std::string& name) const { return m_projects.GetProject(name); }
    bool AddProject(ProjectPtr project) { return m_projects.AddProject(project); }
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

    void AddMessage(const std::string& messageMode, const std::string& message);
};

} // namespace cmake_parser