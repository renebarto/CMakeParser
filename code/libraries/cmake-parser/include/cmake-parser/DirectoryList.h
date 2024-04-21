#pragma once

#include <map>
#include "cmake-parser/Directory.h"

namespace cmake_parser {

using Directories = std::map<std::filesystem::path, DirectoryPtr>;

class DirectoryList
{
private:
    Directories m_directories;
    DirectoryPtr m_rootDirectory;

public:
    DirectoryList();

    const Directories& GetDirectories() const { return m_directories; }
    std::filesystem::path GetDirectory(const std::filesystem::path& path) const;
    DirectoryPtr FindDirectory(const std::filesystem::path& path) const;
    bool AddDirectory(DirectoryPtr directory);

    Directories GetSubDirectories(DirectoryPtr parentDirectory);
    DirectoryPtr GetParentDirectory(DirectoryPtr directory) const;
    DirectoryPtr GetRootDirectory() const { return m_rootDirectory; }

    std::string Serialize() const;
};

inline std::ostream& operator << (std::ostream& stream, const DirectoryList& value)
{
    return stream << value.Serialize();
}

} // namespace cmake_parser