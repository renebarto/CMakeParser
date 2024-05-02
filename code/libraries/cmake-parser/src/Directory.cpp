#include "cmake-parser/Directory.h"

#include <sstream>
#include "tracing/Tracing.h"

using namespace cmake_parser;

Directory::Directory(const std::filesystem::path& sourcePath, const std::filesystem::path& binaryPath)
    : m_sourcePath{ sourcePath }
    , m_binaryPath{ binaryPath }
    , m_variables{}
    , m_parentDirectory{}
{
    TRACE_DEBUG("Create directory {}", Serialize());
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
    TRACE_DEBUG("Create directory {}", Serialize());
}

Directory::~Directory()
{
    TRACE_DEBUG("Delete directory {}", Serialize());
}

std::string Directory::SerializeShort() const
{
    std::ostringstream stream;
    stream << "Directory sourcePath = " << m_sourcePath << ", binaryPath = " << m_binaryPath;
    return stream.str();
}

std::string Directory::Serialize() const
{
    std::ostringstream stream;
    auto parent = m_parentDirectory.lock();
    stream << "Directory sourcePath = " << m_sourcePath << ", binaryPath = " << m_binaryPath << ", parent = (" << ((parent != nullptr) ? parent->SerializeShort() : "none") << ")";
    return stream.str();
}

