#pragma once

#include <map>
#include "cmake-parser/Project.h"

namespace cmake_parser {

using Projects = std::map<std::string, ProjectPtr>;

class ProjectList
{
private:
    Projects m_projects;
    ProjectPtr m_mainProject;

public:
    ProjectList();

    const Projects& GetProjects() const { return m_projects; }
    const ProjectPtr GetProject(const std::string& name) const;
    bool AddProject(ProjectPtr project);

    Projects GetSubProjects(ProjectPtr parentProject);
    const ProjectPtr GetParentProject(ProjectPtr project) const;
    const ProjectPtr GetMainProject() const { return m_mainProject; }

    std::string Serialize() const;

private:
    ProjectPtr FindProject(const std::string& name) const;
    void AddProject(const std::string& name, ProjectPtr project);
};

inline std::ostream& operator << (std::ostream& stream, const ProjectList& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser