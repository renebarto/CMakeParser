#include "cmake-parser/Directory.h"

#include <sstream>
#include "utility/Console.h"

using namespace cmake_parser;

static utility::Console s_console;

Directory::Directory(const std::filesystem::path& sourcePath, const std::filesystem::path& binaryPath)
    : m_sourcePath{ sourcePath }
    , m_binaryPath{ binaryPath }
    , m_variables{}
    , m_parentDirectory{}
{
}

Directory::Directory(const std::filesystem::path& sourcePath, const std::filesystem::path& binaryPath, DirectoryPtr parent)
    : m_sourcePath{ sourcePath }
    , m_binaryPath{ binaryPath }
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
    stream << "Directory sourcePath = " << m_sourcePath << ", binaryPath = " << m_binaryPath << ", parent = (" << ((parent != nullptr) ? parent->Serialize() : "none") << ")";
    return stream.str();
}

