#include "cmake-parser/ProjectList.h"

#include <sstream>
#include "tracing/Tracing.h"

using namespace cmake_parser;

ProjectList::ProjectList()
    : m_projects{}
    , m_mainProject{}
{
}

const ProjectPtr ProjectList::GetProject(const std::string& name) const
{
    auto it = m_projects.find(name);
    return (it == m_projects.end()) ? nullptr : it->second;
}

bool ProjectList::AddProject(ProjectPtr project)
{
    if (project == nullptr)
        return false;
    if (GetProject(project->Name()) != nullptr)
        return false;
    if (project->Parent() == nullptr)
    {
        if (m_mainProject == nullptr)
        {
            TRACE_DEBUG("Set main project {}", project->Name());
            m_mainProject = project;
        }
        else
        {
            TRACE_WARNING("Main project already set when adding project {}", project->Name());
        }
    }
    else
    {
        if (GetProject(project->Parent()->Name()) != project->Parent())
            return false;
    }
    auto parent = project->Parent();
    std::string parentName = (parent == nullptr) ? "none" : parent->Name();
    TRACE_DEBUG("Add project {} parent {}", project->Name(), parentName);
    AddProject(project->Name(), project);
    return true;
}

Projects ProjectList::GetSubProjects(ProjectPtr parentProject)
{
    Projects result;
    for (auto item : m_projects)
    {
        if (item.second != nullptr)
        {
            if (item.second->Parent() == parentProject)
            {
                result.insert(item);
            }
        }
    }
    return result;
}

const ProjectPtr ProjectList::GetParentProject(ProjectPtr project) const
{
    return project->Parent();
}

ProjectPtr ProjectList::FindProject(const std::string& name) const
{
    auto it = m_projects.find(name);
    return (it == m_projects.end()) ? nullptr : it->second;
}

void ProjectList::AddProject(const std::string& name, ProjectPtr project)
{
    m_projects.insert(std::make_pair(name, project));
}

std::string ProjectList::Serialize() const
{
    std::ostringstream stream;
    stream << "ProjectList:" << std::endl;
    for (auto const& proj : m_projects)
    {
        if (proj.second != nullptr)
            stream << proj.second->Serialize() << std::endl;
    }
    return stream.str();
}

