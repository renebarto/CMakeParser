#include "cmake-parser/Directory.h"

#include <sstream>
#include "utility/Console.h"

using namespace cmake_parser;

static utility::Console s_console;

Directory::Directory(const std::filesystem::path& path)
    : m_path{ path }
    , m_variables{}
    , m_parentDirectory{}
{
}

Directory::Directory(const std::filesystem::path& path, DirectoryPtr parent)
    : m_path{ path }
    , m_variables{}
    , m_parentDirectory{ parent }
{
    if (parent != nullptr)
    {
        m_variables = parent->GetVariableList();
    }
}

std::string Directory::Serialize() const
{
    std::ostringstream stream;
    auto parent = m_parentDirectory.lock();
    stream << "Directory path = " << m_path << ", parent = " << ((parent != nullptr) ? parent->Path() : "none");
    return stream.str();
}

