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
    stream << "Directory sourcePath = " << m_sourcePath.generic_string() << ", binaryPath = " << m_binaryPath.generic_string();
    return stream.str();
}

std::string Directory::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    auto parent = m_parentDirectory.lock();
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "Directory sourcePath = " << m_sourcePath.generic_string() << ", binaryPath = " << m_binaryPath.generic_string() << ", parent = (" << ((parent != nullptr) ? parent->SerializeShort() : "none") << ")";
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"sourcePath\": " << SerializeString(format, SourcePath().generic_string()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"binaryPath\": " << SerializeString(format, BinaryPath().generic_string()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"parent\": ";
        if (parent)
        {
            stream << SerializeString(format, parent->SourcePath().generic_string());
        }
        else
        {
            stream << "null";
        }
        stream << std::endl;
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

