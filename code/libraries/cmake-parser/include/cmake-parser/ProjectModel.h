#pragma once

#include <string>
#include <memory>

namespace cmake_parser {

class ProjectModel
{
private:
    std::string m_name;

public:
    ProjectModel(const std::string& name);

    const std::string& Name() const { return m_name; }
};
using ProjectModelPtr = std::shared_ptr<ProjectModel>;

} // namespace cmake_parser