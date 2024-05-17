#include "cmake-parser/Target.h"

#include <sstream>
#include "utility/Serialization.h"
#include "utility/StringFunctions.h"
#include "tracing/Tracing.h"
#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/Directory.h"
#include "cmake-parser/Project.h"

namespace serialization {

template<>
const BidirectionalMap<cmake_parser::TargetType, std::string> EnumSerializationMap<cmake_parser::TargetType>::ConversionMap =
{
    { cmake_parser::TargetType::None, "None" },
    { cmake_parser::TargetType::Executable, "Executable" },
    { cmake_parser::TargetType::StaticLibrary, "StaticLibrary" },
    { cmake_parser::TargetType::SharedLibrary, "SharedLibrary" },
    { cmake_parser::TargetType::Object, "Object" },
};

} // namespace serialization

namespace cmake_parser {

BuildEntry::BuildEntry(const std::filesystem::path& rootDirectory, const std::filesystem::path& binaryDirectory, const std::string& sourcePath)
    : m_rootPath{ rootDirectory }
    , m_binaryPath{ binaryDirectory }
    , m_sourcePath{ rootDirectory / sourcePath }
{
}

std::string SerializeTargetAttribute(const TargetAttribute& value)
{
    if (value == TargetAttribute::None)
        return "None";
    std::string result;
    if (value & TargetAttribute::Win32Exe)
    {
        result += "Win32Exe";
    }
    if (value & TargetAttribute::MacOSXBundle)
    {
        if (!result.empty())
            result += "|";
        result += "MacOSXBundle";
    }
    if (value & TargetAttribute::ExcludeFromAll)
    {
        if (!result.empty())
            result += "|";
        result += "ExcludeFromAll";
    }
    if (value & TargetAttribute::Imported)
    {
        if (!result.empty())
            result += "|";
        result += "Imported";
    }
    if (value & TargetAttribute::Global)
    {
        if (!result.empty())
            result += "|";
        result += "Global";
    }
    if (value & TargetAttribute::Alias)
    {
        if (!result.empty())
            result += "|";
        result += "Alias";
    }
    return result;
}

const std::string TargetPropertyOutputName{ "OUTPUT_NAME" };
const std::string TargetPropertyIncludeDirectories{ "INCLUDE_DIRECTORIES" };
const std::string TargetPropertyInterfaceIncludeDirectories{ "INTERFACE_INCLUDE_DIRECTORIES" };
const std::string TargetPropertyCompileDefinitions{ "COMPILE_DEFINITIONS" };
const std::string TargetPropertyInterfaceCompileDefinitions{ "INTERFACE_COMPILE_DEFINITIONS" };
const std::string TargetPropertyCompileOptions{ "COMPILE_OPTIONS" };
const std::string TargetPropertyInterfaceCompileOptions{ "INTERFACE_COMPILE_OPTIONS" };

extern std::string ExecutableTargetName(const std::string& targetName)
{
    return targetName + ".exe";
}

extern std::string StaticLibraryTargetName(const std::string& targetName)
{
    return targetName + ".lib";
}

extern std::string SharedLibraryTargetName(const std::string& targetName)
{
    return targetName + ".dll";
}

extern std::string ObjectLibraryTargetName(const std::string& targetName)
{
    return targetName + ".obj";
}

Target::Target(ProjectPtr project)
    : m_name{}
    , m_type{}
    , m_attributes{}
    , m_sources{}
    , m_aliasTarget{}
    , m_project{ project }
{
}

Target::Target(ProjectPtr project, const std::string& name, TargetType type, TargetAttribute attributes/* = TargetAttribute::None*/, const std::string& sources /*= {}*/, const std::string& aliasTarget /*= {}*/)
    : m_name{ name }
    , m_type{ type }
    , m_attributes{ attributes }
    , m_sources{ sources }
    , m_aliasTarget{ aliasTarget }
    , m_project{ project }
{
}

void Target::SetProperty(const std::string& name, const std::string& value)
{
    TRACE_DATA("Add or update property {} = {}", name, value);

    m_properties.SetVariable(name, value);
}

void Target::UnsetProperty(const std::string& name)
{
    TRACE_DATA("Unset property {} = {}", name);

    m_properties.UnsetVariable(name);
}

const Variables& Target::GetVariables() const
{
    auto directory = GetDirectory();
    if (directory != nullptr)
    {
        return directory->GetVariables();
    }
    throw std::runtime_error("Undefined directory for target " + Name());
}

std::string Target::GetVariable(const std::string& name) const
{
    auto directory = GetDirectory();
    if (directory != nullptr)
    {
        return directory->GetVariable(name);
    }
    throw std::runtime_error("Undefined directory for target " + Name());
}

void Target::SetVariable(const std::string& name, const std::string& value)
{
    auto directory = GetDirectory();
    if (directory != nullptr)
    {
        directory->SetVariable(name, value);
    }
    throw std::runtime_error("Undefined directory for target " + Name());
}

void Target::UnsetVariable(const std::string& name)
{
    auto directory = GetDirectory();
    if (directory != nullptr)
    {
        directory->UnsetVariable(name);
    }
    throw std::runtime_error("Undefined directory for target " + Name());
}

std::string Target::CompileDefinitions() const
{
    return GetProperty(TargetPropertyCompileDefinitions);
}

std::string Target::InterfaceCompileDefinitions() const
{
    return GetProperty(TargetPropertyInterfaceCompileDefinitions);
}

std::string Target::CompileOptions() const
{
    return GetProperty(TargetPropertyCompileOptions);
}

std::string Target::InterfaceCompileOptions() const
{
    return GetProperty(TargetPropertyCompileOptions);
}

BuildEntries Target::GetBuildEntries() const
{
    BuildEntries result;

    auto directory = GetDirectory();

    std::filesystem::path rootDirectory = GetVariable(VarCurrentSourceDirectory);
    std::filesystem::path binaryDirectory = GetVariable(VarCurrentBinaryDirectory);

    for (auto const& source : Sources().Items())
    {
        BuildEntry buildEntry(rootDirectory, binaryDirectory, source);
        buildEntry.SetCompileDefinitions(CompileDefinitions());
        buildEntry.SetInterfaceCompileDefinitions(InterfaceCompileDefinitions());
        buildEntry.SetCompileOptions(CompileOptions());
        buildEntry.SetInterfaceCompileOptions(InterfaceCompileOptions());
        result.push_back(buildEntry);
    }
    return result;
}

std::string Target::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "Target " << Name() << ":" << serialization::Serialize(Type()) << " [" << Sources().ToString() << "]" << " Attributes " << Attributes() << (IsAlias() ? " " + AliasTarget() : "");
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"name\": " << SerializeString(format, Name()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"type\": " << SerializeString(format, serialization::Serialize(Type())) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"attributes\": \"" << Attributes() << "\"," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"sources\": " << std::endl << Sources().Serialize(format, indent + 8) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"properties\": " << std::endl << SerializeList(format, indent + 8, m_properties);
        if (IsAlias())
        {
            stream << "," << std::endl;
            stream << std::string(indent + 4, ' ') << "\"aliasTarget\": " << SerializeString(format, AliasTarget()) << std::endl;
        }
        else
        {
            stream << std::endl;
        }
        stream << std::string(indent, ' ') << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

DirectoryPtr Target::GetDirectory() const
{
    auto project = Project();
    return (project != nullptr) ? project->Directory() : nullptr;
}

} // namespace cmake_parser
