#pragma once

#include <filesystem>
#include <vector>
#include "cmake-parser/CMakeCache.h"
#include "cmake-parser/DirectoryList.h"
#include "cmake-parser/DirectoryStack.h"
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

// Variable names
extern const std::string VarMinimumRequiredVersion;
extern const std::string VarMainSourceDirectory;
extern const std::string VarMainBinaryDirectory;
extern const std::string VarMainProjectHomePageURL;
extern const std::string VarMainProjectDescription;
extern const std::string VarCurrentSourceDirectory;
extern const std::string VarCurrentBinaryDirectory;
extern const std::string VarCurrentScriptDirectory;
extern const std::string VarCurrentScriptPath;
extern const std::string VarParentScriptPath;
extern const std::string VarCMakeExePath;
extern const std::string VarCPackExePath;
extern const std::string VarCTestExePath;
extern const std::string VarCMakeRootPath;
extern const std::string VarCMakeVersion;
extern const std::string VarCMakeVersionMajor;
extern const std::string VarCMakeVersionMinor;
extern const std::string VarCMakeVersionPatch;
extern const std::string VarCMakeVersionTweak;
extern const std::string VarMakeProgramPath;
extern const std::string VarCMakeGenerator;
extern const std::string VarProjectBinaryDir;
extern const std::string VarProjectDescription;
extern const std::string VarProjectHomepageURL;
extern const std::string VarProjectIsTopLevel;
extern const std::string VarProjectName;
extern const std::string VarProjectSourceDirectory;
extern const std::string VarProjectVersion;
extern const std::string VarProjectVersionMajor;
extern const std::string VarProjectVersionMinor;
extern const std::string VarProjectVersionPatch;
extern const std::string VarProjectVersionTweak;
extern const std::string CMakeScriptFileName;
extern const std::string VarSuffixBinaryDir;
extern const std::string VarSuffixDescription;
extern const std::string VarSuffixHomepageURL;
extern const std::string VarSuffixIsTopLevel;
extern const std::string VarSuffixSourceDirectory;
extern const std::string VarSuffixVersion;
extern const std::string VarSuffixVersionMajor;
extern const std::string VarSuffixVersionMinor;
extern const std::string VarSuffixVersionPatch;
extern const std::string VarSuffixVersionTweak;

class CMakeModel
{
private:
    CMakeCache m_cache;
    VariableList* m_scopeVariables;
    VariableList m_environment;
    ProjectList m_projects;
    ProjectPtr m_rootProject;
    ProjectPtr m_currentProject;
    DirectoryList m_directories;
    DirectoryPtr m_rootDirectory;
    DirectoryStack m_directoryStack;
    std::filesystem::path m_rootSourceDirectory;
    std::filesystem::path m_rootBinaryDirectory;
    bool m_isSourceRootSet;

public:
    CMakeModel();

    bool IsSourceRootSet() const { return m_isSourceRootSet; }
    bool SetupSourceRoot(const std::filesystem::path& root, const std::string& buildDir);
    void SetupCMakePath(const std::filesystem::path& cmakePath, const std::string& cmakeVersion);
    void SetupNinjaPath(const std::filesystem::path& ninjaPath);
    bool EnterDirectory(const std::string& directoryName);
    void LeaveDirectory();

    const TypedVariables& GetCacheVariables() const { return m_cache.GetVariables(); }
    const Variables& GetVariables() const;
    const Variables& GetEnvironmentVariables() const { return m_environment.GetVariables(); }
    std::string GetCacheVariable(const std::string& name) const { return m_cache.GetVariable(name); }
    TypedVariablePtr FindCacheVariable(const std::string& name) const { return m_cache.FindVariable(name); }

    std::string GetVariable(const std::string& name) const;
    VariablePtr FindVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value, VariableAttribute attributes = {}, const std::string& type = {}, const std::string& description = {});
    void UnsetVariable(const std::string& name, VariableAttribute attributes = {});

    std::string GetEnvironmentVariable(const std::string& name) const { return m_environment.GetVariable(name); }
    VariablePtr FindEnvironmentVariable(const std::string& name) const { return m_environment.FindVariable(name); }
    void SetEnvironmentVariable(const std::string& name, const std::string& value);
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

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;

private:
    std::string Evaluate(const std::string& value);
};

inline std::ostream& operator << (std::ostream& stream, const CMakeModel& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser