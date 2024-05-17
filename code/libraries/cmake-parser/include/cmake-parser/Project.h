#pragma once

#include <memory>
#include <string>
#include "cmake-parser/TargetList.h"

namespace cmake_parser {

class Directory;
using DirectoryPtr = std::shared_ptr<Directory>;

class Project
{
private:
    std::string m_name;
    std::string m_version;
    std::string m_description;
    std::string m_languages;
    std::string m_homePageURL;
    std::weak_ptr<Project> m_parentProject;
    TargetList m_targets;
    std::weak_ptr<Directory> m_directory;

public:
    Project() = delete;
    Project(DirectoryPtr directory);
    Project(DirectoryPtr directory, const std::string& name);
    Project(DirectoryPtr directory, const std::string& name, std::shared_ptr<Project> parentProject);

    const std::string& Name() const { return m_name; }
    const std::string& Version() const { return m_version; }
    const std::string& Description() const { return m_description; }
    const std::string& Languages() const { return m_languages; }
    const std::string& HomePageURL() const { return m_homePageURL; }
    std::shared_ptr<Project> Parent() const { return m_parentProject.lock(); }
    DirectoryPtr Directory() const { return m_directory.lock(); }

    void SetVersion(const std::string& version) { m_version = version; }
    void SetDescription(const std::string& description) { m_description = description; }
    void SetLanguages(const std::string& languages) { m_languages = languages; }
    void SetHomePageURL(const std::string& url) { m_homePageURL = url; }
    void SetParent(std::shared_ptr<Project> parent) { m_parentProject = parent; }
    Targets GetTargets() { return m_targets.GetTargets(); }
    void AddTarget(TargetPtr target) { m_targets.AddTarget(target->Name(), target); }

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;
};
using ProjectPtr = std::shared_ptr<Project>;
using WeakProjectPtr = std::weak_ptr<Project>;

inline std::ostream& operator << (std::ostream& stream, const Project& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const ProjectPtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser