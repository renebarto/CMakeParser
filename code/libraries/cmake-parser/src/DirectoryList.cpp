#include "cmake-parser/DirectoryList.h"

#include <sstream>
#include "tracing/Tracing.h"

using namespace cmake_parser;

DirectoryList::DirectoryList()
    : m_directories{}
    , m_rootDirectory{}
{
}

std::filesystem::path DirectoryList::GetDirectory(const std::filesystem::path& path) const
{
    auto dir = FindDirectory(path);
    if (!dir)
        return {};
    return dir->SourcePath();
}

DirectoryPtr DirectoryList::FindDirectory(const std::filesystem::path& path) const
{
    auto it = m_directories.find(path);
    return (it == m_directories.end()) ? nullptr : it->second;
}

bool DirectoryList::AddDirectory(DirectoryPtr directory)
{
    bool result{};
    if (directory == nullptr)
        return false;
    if (FindDirectory(directory->SourcePath()) != nullptr)
        return false;
    if (directory->Parent() == nullptr)
    {
        if (m_rootDirectory == nullptr)
        {
            TRACE_DEBUG("Set main project {}", directory->SourcePath().string());
            m_rootDirectory = directory;
        }
        else
        {
            TRACE_WARNING("Main project already set when adding project {}", directory->SourcePath().string());
        }
    }
    else
    {
        if (FindDirectory(directory->Parent()->SourcePath()) != directory->Parent())
            return false;
    }
    std::string parentPath = (directory->Parent() != nullptr) ? directory->Parent()->SourcePath().string() : "";

    TRACE_DEBUG("Add directory {} parent {}", directory->SourcePath().string(), parentPath);
    m_directories.insert(std::make_pair(directory->SourcePath(), directory));
    return true;
}

Directories DirectoryList::GetSubDirectories(DirectoryPtr parentDirectory)
{
    Directories result;
    for (auto const& dir : m_directories)
    {
        if (dir.second != nullptr)
        {
            if (dir.second->Parent() == parentDirectory)
            {
                result.insert(std::make_pair(dir.first, dir.second));
            }
        }
    }
    return result;
}

DirectoryPtr DirectoryList::GetParentDirectory(DirectoryPtr directory) const
{
    if (!directory)
        return {};
    return directory->Parent();
}

std::string DirectoryList::Serialize() const
{
    std::ostringstream stream;
    stream << "DirectoryList:" << std::endl;
    for (auto const& dir : m_directories)
    {
        if (dir.second != nullptr)
            stream << dir.second->Serialize() << std::endl;
    }
    return stream.str();
}

