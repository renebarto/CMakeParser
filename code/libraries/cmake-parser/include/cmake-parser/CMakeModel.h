#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include "cmake-parser/ProjectModel.h"

namespace cmake_parser {

struct Attribute
{
    std::string name;
    std::string value;

    Attribute(const std::string& name, const std::string& value)
        : name{ name }
        , value{ value }
    {
    }
};
using AttributePtr = std::shared_ptr<Attribute>;
using AttributeList = std::map<std::string, AttributePtr>;
struct Variable
{
    std::string name;
    std::string value;

    Variable(const std::string& name, const std::string& value)
        : name{ name }
        , value{ value }
    {
    }
};
using VariablePtr = std::shared_ptr<Variable>;
using VariableList = std::map<std::string, VariablePtr>;
using ProjectModelList = std::map<std::string, ProjectModelPtr>;

class CMakeModel
{
private:
    VariableList m_variables;
    AttributeList m_attributes;
    ProjectModelList m_projects;
    ProjectModelPtr m_mainProject;

public:
    CMakeModel();

    void SetupRootCMakeFile(const std::string& rootListFile);
    void SetupSourceRoot(const std::filesystem::path& root);
    void SetupCMakePath(const std::string& cmakePath, const std::string& cmakeVersion);
    void SetupNinjaPath(const std::string& ninjaPath);
    bool AddProject(ProjectModelPtr project);

    const VariableList& GetVariables() const { return m_variables; }
    std::string GetVariable(const std::string& name) const;
    void SetVariable(const std::string& name, const std::string& value);
    const AttributeList& GetAttributes() const { return m_attributes; }
    const AttributePtr GetAttribute(const std::string& name) const;
    void SetAttribute(const std::string& name, const std::string& value);
    const ProjectModelList& GetProjects() const { return m_projects; }
    const ProjectModelPtr GetProject(const std::string& name) const;
    const ProjectModelPtr GetMainProject() const { return m_mainProject; }
    void AddMessage(const std::string& messageMode, const std::string& message);


private:
    VariablePtr FindVariable(const std::string& name);
    AttributePtr FindAttribute(const std::string& name);
};

} // namespace cmake_parser