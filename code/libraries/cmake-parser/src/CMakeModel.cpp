#include "cmake-parser/CMakeModel.h"

#include <iostream>
#include "utility/Console.h"

using namespace cmake_parser;

#define VAR(name, value) { #name, std::make_shared<Variable>(#name, value) }

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

static utility::Console s_console;

static const VariableList DefaultVariables
{
    VAR(ARGC, "0"),
    VAR(ARGN, ""),
    VAR(ARGV, ""),
    VAR(CMAKE_BINARY_DIR, ""),
    VAR(CMAKE_BUILD_TYPE, "Debug"),
    VAR(CMAKE_COMMAND, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"),
    VAR(CMAKE_CPACK_COMMAND, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cpack.exe"),
    VAR(CMAKE_CTEST_COMMAND, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe"),
    VAR(CMAKE_CURRENT_BINARY_DIR, ""),
    VAR(CMAKE_CURRENT_FUNCTION, ""),
    VAR(CMAKE_CURRENT_FUNCTION_LIST_DIR, ""),
    VAR(CMAKE_CURRENT_FUNCTION_LIST_FILE, ""),
    VAR(CMAKE_CURRENT_FUNCTION_LIST_LINE, ""),
    VAR(CMAKE_CURRENT_LIST_DIR, ""),
    VAR(CMAKE_CURRENT_LIST_FILE, ""),
    VAR(CMAKE_CURRENT_SOURCE_DIR, ""),
    VAR(CMAKE_CXX_COMPILER, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe"),
    VAR(CMAKE_C_COMPILER, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe"),
    VAR(CMAKE_EXTRA_GENERATOR, ""),
    VAR(CMAKE_FILES_DIRECTORY, "/CMakeFiles"),
    VAR(CMAKE_GENERATOR, "Ninja"),
    VAR(CMAKE_GENERATOR_INSTANCE, ""),
    VAR(CMAKE_GENERATOR_PLATFORM, ""),
    VAR(CMAKE_GENERATOR_TOOLSET, ""),
    VAR(CMAKE_HOME_DIRECTORY, ""),
    VAR(CMAKE_HOST_SYSTEM_NAME, "Windows"),
    VAR(CMAKE_HOST_WIN32, "1"),
    VAR(CMAKE_INSTALL_PREFIX, ""),
    VAR(CMAKE_MAJOR_VERSION, "3"),
    VAR(CMAKE_MAKE_PROGRAM, "C:\\PROGRAM FILES(X86)\\MICROSOFT VISUAL STUDIO\\2019\\COMMUNITY\\COMMON7\\IDE\\COMMONEXTENSIONS\\MICROSOFT\\CMAKE\\Ninja\\ninja.exe"),
    VAR(CMAKE_MINIMUM_REQUIRED_VERSION, ""),
    VAR(CMAKE_MINOR_VERSION, "20"),
    VAR(CMAKE_MODULE_PATH, ""),
    VAR(CMAKE_PARENT_LIST_FILE, ""),
    VAR(CMAKE_PATCH_VERSION, "21032501"),
    VAR(CMAKE_ROOT, "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20"),
    VAR(CMAKE_SOURCE_DIR, ""),
    VAR(CMAKE_TWEAK_VERSION, "0"),
    VAR(CMAKE_VERSION, "3.20.21032501-MSVC_2"),
    VAR(WIN32, "1"),
};

CMakeModel::CMakeModel()
    : m_variables{}
    , m_attributes{}
    , m_projects{}
    , m_mainProject{}
{
    for (auto var : DefaultVariables)
    {
        m_variables.insert(std::make_pair(var.first, std::make_shared<Variable>(var.first, var.second->value)));
    }
}

void CMakeModel::SetupRootCMakeFile(const std::string& rootListFile)
{
    std::filesystem::path rootCMakeFile = rootListFile;
    std::filesystem::path rootCMakeDir = rootListFile;
    if (rootCMakeFile.has_filename())
        rootCMakeDir.remove_filename();
    else
        rootCMakeDir /= "CMakeLists.txt";
    rootCMakeDir = rootCMakeDir.parent_path();
    SetVariable("CMAKE_CURRENT_LIST_DIR", rootCMakeDir.generic_string());
    SetVariable("CMAKE_CURRENT_LIST_FILE", rootCMakeFile.generic_string());
    SetVariable("CMAKE_PARENT_LIST_FILE", rootCMakeFile.generic_string());
    SetupSourceRoot(rootCMakeDir);
}

void CMakeModel::SetupSourceRoot(const std::filesystem::path& root)
{
    auto binaryDir = root / "cmake-x64-Debug";

    SetVariable("CMAKE_SOURCE_DIR", root.generic_string());
    SetVariable("CMAKE_CURRENT_SOURCE_DIR", root.generic_string());
    SetVariable("CMAKE_BINARY_DIR", binaryDir.generic_string());
    SetVariable("CMAKE_CURRENT_BINARY_DIR", binaryDir.generic_string());
}

void CMakeModel::SetupCMakePath(const std::string& cmakePath, const std::string& cmakeVersion)
{
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
    SetVariable("CMAKE_COMMAND", cmakeExePath.generic_string());
    SetVariable("CMAKE_CPACK_COMMAND", cpackExePath.generic_string());
    SetVariable("CMAKE_CTEST_COMMAND", ctestExePath.generic_string());
    SetVariable("CMAKE_ROOT", cmakeRoot.generic_string());
    SetVariable("CMAKE_VERSION", cmakeVersion);
    SetVariable("CMAKE_MAJOR_VERSION", versionMajor);
    SetVariable("CMAKE_MINOR_VERSION", versionMinor);
    SetVariable("CMAKE_PATCH_VERSION", versionPatch);
    SetVariable("CMAKE_TWEAK_VERSION", versionTweak);
}

void CMakeModel::SetupNinjaPath(const std::string& ninjaPath)
{
    std::filesystem::path ninjaBinDir = ninjaPath;
    if (ninjaBinDir.has_filename())
        ninjaBinDir.remove_filename();
    auto ninjaExePath = ninjaBinDir / "ninja.exe";
    ninjaExePath.make_preferred();
    SetVariable("CMAKE_MAKE_PROGRAM", ninjaExePath.string());
    SetVariable("CMAKE_GENERATOR", "Ninja");
}

bool CMakeModel::AddProject(ProjectModelPtr project)
{
    if (GetProject(project->Name()) != nullptr)
        return false;
    if (m_mainProject == nullptr)
    {
        s_console << fgcolor(utility::ConsoleColor::Cyan) << "Set main project " << project->Name() << fgcolor(utility::ConsoleColor::Default) << std::endl;
        m_mainProject = project;
    }
    auto projectEntry = std::make_pair(project->Name(), project);
    s_console << fgcolor(utility::ConsoleColor::Cyan) << "Add project " << project->Name() << fgcolor(utility::ConsoleColor::Default) << std::endl;
    m_projects.insert(projectEntry);
    return true;
}

std::string CMakeModel::GetVariable(const std::string& name) const
{
    auto it = m_variables.find(name);
    if (it == m_variables.end())
        return {};
    return it->second->value;
}

void CMakeModel::SetVariable(const std::string& name, const std::string& value)
{
    auto var = FindVariable(name);
    if (var == nullptr)
    {
        s_console << fgcolor(utility::ConsoleColor::Cyan) << "Add new variable " << name << " = " << value << fgcolor(utility::ConsoleColor::Default) << std::endl;
        var = std::make_shared<Variable>(name, value);
        m_variables.insert(std::pair(name, var));
    }
    else
    {
        s_console << fgcolor(utility::ConsoleColor::Cyan) << "Update variable " << name << " = " << value << fgcolor(utility::ConsoleColor::Default) << std::endl;
        var->value = value;
    }
}

const AttributePtr CMakeModel::GetAttribute(const std::string& name) const
{
    auto it = m_attributes.find(name);
    return (it == m_attributes.end()) ? nullptr : it->second;
}

void CMakeModel::SetAttribute(const std::string& name, const std::string& value)
{
    auto attribute = FindAttribute(name);
    if (attribute == nullptr)
    {
        s_console << fgcolor(utility::ConsoleColor::Cyan) << "Add new attribute " << name << " = " << value << fgcolor(utility::ConsoleColor::Default) << std::endl;
        attribute = std::make_shared<Attribute>(name, value);
        m_attributes.insert(std::pair(name, attribute));
    }
    else
    {
        s_console << fgcolor(utility::ConsoleColor::Cyan) << "Update attribute " << name << " = " << value << fgcolor(utility::ConsoleColor::Default) << std::endl;
        attribute->value = value;
    }
}

const ProjectModelPtr CMakeModel::GetProject(const std::string& name) const
{
    auto it = m_projects.find(name);
    return (it == m_projects.end()) ? nullptr : it->second;
}

VariablePtr CMakeModel::FindVariable(const std::string& name)
{
    auto it = m_variables.find(name);
    return (it == m_variables.end()) ? nullptr : it->second;
}

AttributePtr CMakeModel::FindAttribute(const std::string& name)
{
    auto it = m_attributes.find(name);
    return (it == m_attributes.end()) ? nullptr : it->second;
}

void CMakeModel::AddMessage(const std::string& messageMode, const std::string& message)
{
    s_console << fgcolor(utility::ConsoleColor::Yellow) << "Message(" << messageMode << " " << message << ")" << fgcolor(utility::ConsoleColor::Default) << std::endl;
}
