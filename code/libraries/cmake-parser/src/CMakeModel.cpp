#include "cmake-parser/CMakeModel.h"

#include <cassert>
#include <iostream>
#include "utility/StringFunctions.h"
#include "tracing/Tracing.h"
#include "cmake-parser/Expression.h"

namespace cmake_parser {

// Variables set by cmake inside Visual Studio 2019. This may change
// ARGC=0
// ARGN=
// ARGV=
// CMAKE_BINARY_DIR=D:/Projects/CPPParser/cmake-x64-Debug
// CMAKE_BUILD_TYPE=Debug
// CMAKE_COMMAND=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe
// CMAKE_CPACK_COMMAND=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cpack.exe
// CMAKE_CTEST_COMMAND=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe
// CMAKE_CURRENT_BINARY_DIR=D:/Projects/CPPParser/cmake-x64-Debug
// CMAKE_CURRENT_FUNCTION=dump_all_vars
// CMAKE_CURRENT_FUNCTION_LIST_DIR=D:/Projects/CPPParser
// CMAKE_CURRENT_FUNCTION_LIST_FILE=D:/Projects/CPPParser/CMakeLists.txt
// CMAKE_CURRENT_FUNCTION_LIST_LINE=5
// CMAKE_CURRENT_LIST_DIR=D:/Projects/CPPParser
// CMAKE_CURRENT_LIST_FILE=D:/Projects/CPPParser/CMakeLists.txt
// CMAKE_CURRENT_SOURCE_DIR=D:/Projects/CPPParser
// CMAKE_CXX_COMPILER=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x64/cl.exe
// CMAKE_C_COMPILER=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x64/cl.exe
// CMAKE_EXTRA_GENERATOR=
// CMAKE_FILES_DIRECTORY=/CMakeFiles
// CMAKE_GENERATOR=Ninja
// CMAKE_GENERATOR_INSTANCE=
// CMAKE_GENERATOR_PLATFORM=
// CMAKE_GENERATOR_TOOLSET=
// CMAKE_HOME_DIRECTORY=D:/Projects/CPPParser
// CMAKE_HOST_SYSTEM_NAME=Windows
// CMAKE_HOST_WIN32=1
// CMAKE_INSTALL_PREFIX=D:/Projects/CPPParser/output/install/x64-Debug
// CMAKE_MAJOR_VERSION=3
// CMAKE_MAKE_PROGRAM=C:\PROGRAM FILES (X86)\MICROSOFT VISUAL STUDIO\2019\COMMUNITY\COMMON7\IDE\COMMONEXTENSIONS\MICROSOFT\CMAKE\Ninja\ninja.exe
// CMAKE_MINIMUM_REQUIRED_VERSION=3.5.1
// CMAKE_MINOR_VERSION=20
// CMAKE_PARENT_LIST_FILE=D:/Projects/CPPParser/CMakeLists.txt
// CMAKE_PATCH_VERSION=21032501
// CMAKE_ROOT=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20
// CMAKE_SOURCE_DIR=D:/Projects/CPPParser
// CMAKE_TWEAK_VERSION=0
// CMAKE_VERSION=3.20.21032501-MSVC_2
// VERBOSE_BUILD=OFF
// WIN32=1

// Variable names
const std::string VarMinimumRequiredVersion{ "CMAKE_MINIMUM_REQUIRED_VERSION" };
const std::string VarMainSourceDirectory{ "CMAKE_SOURCE_DIR" };
const std::string VarMainBinaryDirectory{ "CMAKE_BINARY_DIR" };
const std::string VarCurrentSourceDirectory{ "CMAKE_CURRENT_SOURCE_DIR" };
const std::string VarCurrentBinaryDirectory{ "CMAKE_CURRENT_BINARY_DIR" };
const std::string VarCurrentScriptDirectory{ "CMAKE_CURRENT_LIST_DIR" };
const std::string VarCurrentScriptPath{ "CMAKE_CURRENT_LIST_FILE" };
const std::string VarParentScriptPath{ "CMAKE_PARENT_LIST_FILE" };
const std::string VarCMakeExePath{ "CMAKE_COMMAND" };
const std::string VarCPackExePath{ "CMAKE_CPACK_COMMAND" };
const std::string VarCTestExePath{ "CMAKE_CTEST_COMMAND" };
const std::string VarCMakeRootPath{ "CMAKE_ROOT" };
const std::string VarCMakeVersion{ "CMAKE_VERSION" };
const std::string VarCMakeVersionMajor{ "CMAKE_MAJOR_VERSION" };
const std::string VarCMakeVersionMinor{ "CMAKE_MINOR_VERSION" };
const std::string VarCMakeVersionPatch{ "CMAKE_PATCH_VERSION" };
const std::string VarCMakeVersionTweak{ "CMAKE_TWEAK_VERSION" };
const std::string VarMakeProgramPath{ "CMAKE_MAKE_PROGRAM" };
const std::string VarCMakeGenerator{ "CMAKE_GENERATOR" };
const std::string VarProjectBinaryDir{ "PROJECT_BINARY_DIR" };
const std::string VarProjectHomepageURL{ "PROJECT_HOMEPAGE_URL" };
const std::string VarProjectName{ "PROJECT_NAME" };
const std::string VarProjectDescription{ "PROJECT_DESCRIPTION" };
const std::string VarProjectSourceDirectory{ "PROJECT_SOURCE_DIR" };
const std::string VarProjectVersion{ "PROJECT_VERSION" };
const std::string VarProjectVersionMajor{ "PROJECT_VERSION_MAJOR" };
const std::string VarProjectVersionMinor{ "PROJECT_VERSION_MINOR" };
const std::string VarProjectVersionPatch{ "PROJECT_VERSION_PATCH" };
const std::string VarProjectVersionTweak{ "PROJECT_VERSION_TWEAK" };
const std::string CMakeScriptFileName{ "CMakeLists.txt" };

#define VAR(name, value) { #name, value }

static const std::map<std::string, std::string> DefaultVariables
{
    VAR(ARGC, "0"),
    VAR(ARGN, ""),
    VAR(ARGV, ""),
    VAR(CMAKE_BUILD_TYPE, "Debug"),
    VAR(CMAKE_CXX_COMPILER, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe"),
    VAR(CMAKE_C_COMPILER, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe"),
    VAR(CMAKE_FILES_DIRECTORY, "/CMakeFiles"),
    VAR(CMAKE_GENERATOR_INSTANCE, ""),
    VAR(CMAKE_GENERATOR_PLATFORM, ""),
    VAR(CMAKE_GENERATOR_TOOLSET, ""),
    VAR(CMAKE_HOME_DIRECTORY, ""),
    VAR(CMAKE_HOST_SYSTEM_NAME, "Windows"),
    VAR(CMAKE_HOST_WIN32, "1"),
    VAR(WIN32, "1"),
};

struct Version
{
    std::string major;
    std::string minor;
    std::string patch;
    std::string tweak;
};

static Version SplitVersion(const std::string& versionString)
{
    Version version{};

    auto versionParts = utility::Split(versionString, '.');
    version.major = ((versionParts.size() > 0) && !versionParts[0].empty()) ? versionParts[0] : "0";
    version.minor = (versionParts.size() > 1) ? versionParts[1] : "0";
    version.patch = (versionParts.size() > 2) ? versionParts[2] : "0";
    version.tweak = (versionParts.size() > 3) ? versionParts[3] : "0";

    return version;
}

CMakeModel::CMakeModel()
    : m_cache{}
    , m_scopeVariables{}
    , m_projects{}
    , m_rootProject{}
    , m_currentProject{}
    , m_directories{}
    , m_rootDirectory{}
    , m_directoryStack{}
    , m_isSourceRootSet{}
{
}

bool CMakeModel::SetupSourceRoot(const std::filesystem::path& root, const std::string& buildDir)
{
    m_rootSourceDirectory = root;
    m_rootSourceDirectory.make_preferred();
    m_rootBinaryDirectory = (root / buildDir).generic_string();
    m_rootBinaryDirectory.make_preferred();
    m_isSourceRootSet = true;
    m_rootDirectory = std::make_shared<Directory>(m_rootSourceDirectory, m_rootBinaryDirectory);
    std::filesystem::path listFilePath = m_rootSourceDirectory / CMakeScriptFileName;
    listFilePath.make_preferred();

    if (!std::filesystem::exists(listFilePath))
    {
        TRACE_ERROR("File does not exist: {}", listFilePath);
        return false;
    }
    if (!m_directories.AddDirectory(m_rootDirectory))
    {
        TRACE_ERROR("Cannot add directory: {}", m_rootDirectory);
        return false;
    }

    m_directoryStack.Push(m_rootDirectory);
    m_scopeVariables = &m_rootDirectory->GetVariableList();
    SetVariable(VarMainSourceDirectory, m_rootSourceDirectory.generic_string());
    SetVariable(VarMainBinaryDirectory, m_rootBinaryDirectory.generic_string());
    SetVariable(VarCurrentSourceDirectory, m_rootSourceDirectory.generic_string());
    SetVariable(VarCurrentBinaryDirectory, m_rootBinaryDirectory.generic_string());
    SetVariable(VarCurrentScriptDirectory, m_rootSourceDirectory.generic_string());
    SetVariable(VarCurrentScriptPath, listFilePath.generic_string());
    SetVariable(VarParentScriptPath, listFilePath.generic_string());

    for (auto var : DefaultVariables)
    {
        SetVariable(var.first, var.second);
    }
    return true;
}

void CMakeModel::SetupCMakePath(const std::filesystem::path& cmakePath, const std::string& cmakeVersion)
{
    assert(IsSourceRootSet());
    auto versionMajor = cmakeVersion.substr(0, cmakeVersion.find('.', 0));
    auto versionRest = cmakeVersion.substr(cmakeVersion.find('.', 0) + 1);
    auto versionMinor = versionRest.substr(0, versionRest.find('.', 0));
    versionRest = versionRest.substr(versionRest.find('.', 0) + 1);
    auto versionPatch = versionRest.substr(0, versionRest.find('.', 0));
    std::string versionTweak = "0";

    std::filesystem::path cmakeBinDir = cmakePath;
    if (cmakeBinDir.has_filename())
        cmakeBinDir.remove_filename();
    auto cmakeExePath = cmakeBinDir / "cmake.exe";
    auto cpackExePath = cmakeBinDir / "cpack.exe";
    auto ctestExePath = cmakeBinDir / "ctest.exe";
    auto cmakeRoot = std::filesystem::weakly_canonical(cmakeBinDir / ".." / "share" / ("cmake-" + versionMajor + "." + versionMinor));
    SetVariable(VarCMakeExePath, cmakeExePath.generic_string());
    SetVariable(VarCPackExePath, cpackExePath.generic_string());
    SetVariable(VarCTestExePath, ctestExePath.generic_string());
    SetVariable(VarCMakeRootPath, cmakeRoot.generic_string());
    SetVariable(VarCMakeVersion, cmakeVersion);
    SetVariable(VarCMakeVersionMajor, versionMajor);
    SetVariable(VarCMakeVersionMinor, versionMinor);
    SetVariable(VarCMakeVersionPatch, versionPatch);
    SetVariable(VarCMakeVersionTweak, versionTweak);
}

void CMakeModel::SetupNinjaPath(const std::filesystem::path& ninjaPath)
{
    assert(IsSourceRootSet());
    std::filesystem::path ninjaBinDir = ninjaPath;
    if (ninjaBinDir.has_filename())
        ninjaBinDir.remove_filename();
    auto ninjaExePath = ninjaBinDir / "ninja.exe";
    ninjaExePath.make_preferred();
    SetVariable(VarMakeProgramPath, ninjaExePath.string());
    SetVariable(VarCMakeGenerator, "Ninja");
}

bool CMakeModel::EnterDirectory(const std::string& directoryName)
{
    assert(IsSourceRootSet());
    std::filesystem::path parentSourceDirectory = m_rootSourceDirectory;
    std::filesystem::path parentBinaryDirectory = m_rootBinaryDirectory;
    DirectoryPtr parentDirectory = m_directoryStack.At(0);
    if (parentDirectory != nullptr)
    {        
        parentSourceDirectory = parentDirectory->SourcePath();
        parentBinaryDirectory = parentDirectory->BinaryPath();
    }
    auto currentSourceDirectory = parentSourceDirectory / directoryName;
    currentSourceDirectory.make_preferred();
    auto currentBinaryDirectory = parentBinaryDirectory / directoryName;
    currentBinaryDirectory.make_preferred();
    auto listFilePath = currentSourceDirectory / CMakeScriptFileName;
    listFilePath.make_preferred();

    if (!std::filesystem::exists(listFilePath))
    {
        TRACE_ERROR("File does not exist: {}", listFilePath);
        return false;
    }
    auto directory = std::make_shared<Directory>(currentSourceDirectory, currentBinaryDirectory, parentDirectory);
    if (!m_directories.AddDirectory(directory))
    {
        TRACE_ERROR("Cannot add directory: {}", directory);
        return false;
    }
    m_directoryStack.Push(directory);
    m_scopeVariables = &directory->GetVariableList();
    SetVariable(VarCurrentSourceDirectory, currentSourceDirectory.generic_string());
    SetVariable(VarCurrentBinaryDirectory, currentBinaryDirectory.generic_string());
    SetVariable(VarCurrentScriptDirectory, currentSourceDirectory.generic_string());
    SetVariable(VarCurrentScriptPath, listFilePath.generic_string());
    SetVariable(VarParentScriptPath, (parentSourceDirectory / CMakeScriptFileName).generic_string());
    return true;
}

void CMakeModel::LeaveDirectory()
{
    assert(IsSourceRootSet());

    m_directoryStack.Pop();
    m_scopeVariables = &m_directoryStack.Top()->GetVariableList();
}

const Variables& CMakeModel::GetVariables() const
{
    assert(IsSourceRootSet());
    return m_scopeVariables->GetVariables();
}

std::string CMakeModel::GetVariable(const std::string& name) const
{
    assert(IsSourceRootSet());
    return m_scopeVariables->GetVariable(name);
}

VariablePtr CMakeModel::FindVariable(const std::string& name) const
{
    assert(IsSourceRootSet());
    return m_scopeVariables->FindVariable(name);
}

void CMakeModel::SetVariable(const std::string& name, const std::string& value, VariableAttribute attributes /*= {}*/, const std::string& type /*= {}*/, const std::string& description /*= {}*/)
{
    auto evaluatedName = Evaluate(utility::UnQuote(name));
    auto evaluatedValue = Evaluate(utility::UnQuote(value));
    auto evaluatedDescription = Evaluate(utility::UnQuote(description));

    if (attributes & VariableAttribute::Cache)
    {
        auto var = m_cache.FindVariable(evaluatedName);
        if (var == nullptr)
        {
            auto varType = type.empty() ? "STRING" : type;
            TRACE_DATA("Add new cache variable {}:{} = {} ({})", evaluatedName, varType, evaluatedValue, evaluatedDescription);
            var = std::make_shared<TypedVariable>(evaluatedName, varType, evaluatedValue, evaluatedDescription);
            m_cache.AddVariable(evaluatedName, var);
        }
        else
        {
            if (attributes & VariableAttribute::Force)
            {
                if (type.empty())
                {
                    TRACE_DATA("Update cache variable {} = {} ({})", evaluatedName, evaluatedValue, evaluatedDescription);
                    var->SetValue(evaluatedValue);
                    var->SetDescription(evaluatedDescription);
                }
                else
                {
                    TRACE_DATA("Update cache variable {}:{} = {} ({})", evaluatedName, type, evaluatedValue, evaluatedDescription);
                    var->SetValue(type, evaluatedValue);
                    var->SetDescription(evaluatedDescription);
                }
            }
            else
            {
                TRACE_DATA("Not updating cache variable, already set {} = {}", evaluatedName, evaluatedValue);
            }
        }
    }
    else
    {
        assert(IsSourceRootSet());
        auto var = m_scopeVariables->FindVariable(evaluatedName);
        if (var == nullptr)
        {
            TRACE_DATA("Add new variable {} = {}", evaluatedName, evaluatedValue);
            var = std::make_shared<Variable>(evaluatedName, evaluatedValue);
            m_scopeVariables->AddVariable(evaluatedName, var);
        }
        else
        {
            TRACE_DATA("Update variable {} = {}", evaluatedName, evaluatedValue);
            var->SetValue(evaluatedValue);
        }
    }
}

void CMakeModel::UnsetVariable(const std::string& name, VariableAttribute attributes /*= {}*/)
{
    if (attributes & VariableAttribute::Cache)
    {
        TRACE_DATA("Unset cache variable {}", name);
        m_cache.UnsetVariable(name);
    }
    else
    {
        TRACE_DATA("Unset variable {}", name);
        assert(IsSourceRootSet());
        m_scopeVariables->UnsetVariable(name);
    }
}

void CMakeModel::SetEnvironmentVariable(const std::string& name, const std::string& value)
{
    auto evaluatedName = Evaluate(utility::UnQuote(name));
    auto evaluatedValue = Evaluate(utility::UnQuote(value));

    TRACE_DATA("Set environment variable {} = {}", evaluatedName, evaluatedValue);
    m_environment.SetVariable(evaluatedName, evaluatedValue);
}

void CMakeModel::UnsetEnvironmentVariable(const std::string& name)
{
    TRACE_DATA("Unset environment variable {}", name);
    m_environment.UnsetVariable(name);
}

bool CMakeModel::AddProject(ProjectPtr project)
{
    if (project != nullptr)
        TRACE_DATA("Add project {}", project->Name());
    else
        TRACE_ERROR("Adding null project!");
    auto result = m_projects.AddProject(project);
    if (result)
    {
        SetVariable(VarProjectBinaryDir, GetVariable("CMAKE_CURRENT_BINARY_DIR"));
        SetVariable(VarProjectHomepageURL, "");
        SetVariable(VarProjectName, project->Name());
        SetVariable(VarProjectDescription, project->Description());
        SetVariable(VarProjectSourceDirectory, GetVariable(VarCurrentSourceDirectory));
        SetVariable(VarProjectVersion, project->Version());
        auto version = SplitVersion(project->Version());
        SetVariable(VarProjectVersionMajor, version.major);
        SetVariable(VarProjectVersionMinor, version.minor);
        SetVariable(VarProjectVersionPatch, version.patch);
        SetVariable(VarProjectVersionTweak, version.tweak);
    }

    return result;
}

DirectoryPtr CMakeModel::GetCurrentDirectory() const
{
    return m_directoryStack.Top();
}

void CMakeModel::AddMessage(const std::string& messageMode, const std::string& message)
{
    auto evaluatedValue = Evaluate(message);

    TRACE_INFO("Message({} {})", messageMode, evaluatedValue);
}

std::string CMakeModel::Evaluate(const std::string& value)
{
    return expression::Expression::EvaluateString(*this, value);
}

std::string CMakeModel::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream
            << "Model sourceDir = " << m_rootSourceDirectory.generic_string()
            << ", binaryDir = " << m_rootBinaryDirectory.generic_string();
        break;
    case SerializationFormat::JSON:
        stream << "{" << std::endl;
        stream << std::string(indent + 4, ' ') << "\"sourceRoot\": " << SerializeString(format, m_rootSourceDirectory.generic_string()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"binaryRoot\": " << SerializeString(format, m_rootBinaryDirectory.generic_string()) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"cache\": " << std::endl << m_cache.Serialize(format, indent + 8) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"environment\": " << std::endl << m_environment.Serialize(format, indent + 8) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"projects\": " << std::endl << m_projects.Serialize(format, indent + 8) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"directories\": " << std::endl << m_directories.Serialize(format, indent + 8) << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"rootDirectory\": ";
        if (m_rootDirectory == nullptr)
        {
            stream << "null";
        }
        else
        {
            stream << SerializeString(format, m_rootDirectory->SourcePath().generic_string());
        }
        stream << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"rootProject\": ";
        if (m_rootProject == nullptr)
        {
            stream << "null";
        }
        else
        {
            stream << SerializeString(format, m_rootProject->Name());
        }
        stream << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"currentProject\": ";
        if (m_currentProject == nullptr)
        {
            stream << "null";
        }
        else
        {
            stream << SerializeString(format, m_currentProject->Name());
        }
        stream << "," << std::endl;
        stream << std::string(indent + 4, ' ') << "\"currentVariables\": ";
        if (m_scopeVariables == nullptr)
        {
            stream << "null";
        }
        else
        {
            stream << std::endl << m_scopeVariables->Serialize(format, indent + 8);
        }
        stream << std::endl;
        stream << "}";
        break;
    default:
        break;
    }
    return stream.str();
}

} // namespace cmake_parser
