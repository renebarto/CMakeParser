#include "cmake-parser/CMakeModel.h"

#include "test-platform/GoogleTest.h"
#include "utility/CommandLine.h"
#include "utility/Regex.h"
#include "tracing/StringTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {

class CMakeModelTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    StringTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    CMakeModelTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
        Tracing::SetDefaultTraceFilter({ TraceCategory::Information });
    }
    void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

static std::filesystem::path FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return std::filesystem::path(stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0))).remove_filename();
    return {};
}

static std::string GetCMakeVersion()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("cmake --version", "", stdoutText, stderrText) == 0)
    {
        auto line = stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
        std::string prefix = "cmake version ";
        return line.substr(prefix.length());
    }
    return {};
}

static std::filesystem::path FindNinja()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

static const std::string BuildDir{ "cmake-x64-Debug" };

static const std::filesystem::path TOOLCHAIN_PATH{ "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64" };
static const std::string ARGC{ "0" };
static const std::string ARGN{ "" };
static const std::string ARGV{ "" };
static const std::filesystem::path CMAKE_BINARY_DIR{ std::filesystem::path(TEST_DATA_DIR) / "cmake-x64-Debug" };
static const std::string CMAKE_BUILD_TYPE{ "Debug" };
static const std::filesystem::path CMAKE_COMMAND{ (FindCMake() / "cmake.exe") };
static const std::filesystem::path CMAKE_CPACK_COMMAND{ FindCMake() / "cpack.exe" };
static const std::filesystem::path CMAKE_CTEST_COMMAND{ FindCMake() / "ctest.exe" };
static const std::filesystem::path CMAKE_CURRENT_BINARY_DIR{ CMAKE_BINARY_DIR };
static const std::filesystem::path CMAKE_CURRENT_LIST_DIR{ TEST_DATA_DIR };
static const std::filesystem::path CMAKE_CURRENT_LIST_FILE{ CMAKE_CURRENT_LIST_DIR / "CMakeLists.txt" };
static const std::filesystem::path CMAKE_CURRENT_SOURCE_DIR{ TEST_DATA_DIR };
static const std::filesystem::path CMAKE_CXX_COMPILER{ TOOLCHAIN_PATH / "cl.exe" };
static const std::filesystem::path CMAKE_C_COMPILER{ TOOLCHAIN_PATH / "cl.exe" };
static const std::string CMAKE_FILES_DIRECTORY{ "/CMakeFiles" };
static const std::string CMAKE_GENERATOR{ "Ninja" };
static const std::string CMAKE_GENERATOR_INSTANCE{};
static const std::string CMAKE_GENERATOR_PLATFORM{};
static const std::string CMAKE_GENERATOR_TOOLSET{};
static const std::string CMAKE_HOME_DIRECTORY{};
static const std::string CMAKE_HOST_SYSTEM_NAME{ "Windows" };
static const std::string CMAKE_HOST_WIN32{ "1" };
static const std::string CMAKE_MAJOR_VERSION{ "3" };
static const std::filesystem::path CMAKE_MAKE_PROGRAM{ FindNinja() };
static const std::string CMAKE_MINOR_VERSION{ "27" };
static const std::filesystem::path CMAKE_PARENT_LIST_FILE{ CMAKE_CURRENT_LIST_FILE };
static const std::string CMAKE_PATCH_VERSION{ "8" };
static const std::filesystem::path CMAKE_ROOT{ std::filesystem::canonical(FindCMake() / ".." / "share" / "cmake-3.27") };
static const std::filesystem::path CMAKE_SOURCE_DIR{ TEST_DATA_DIR };
static const std::string CMAKE_TWEAK_VERSION{ "0" };
static const std::string CMAKE_VERSION{ "3.27.8" };
static const std::string ISWIN32{ "1" };

TEST_F(CMakeModelTest, Construct)
{
    CMakeModel model;

    EXPECT_FALSE(model.IsSourceRootSet());
    // Cannot retrieve variables as no source root is set yet
    //EXPECT_EQ(size_t{ 0 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 0 }, model.GetDirectories().size());
}

TEST_F(CMakeModelTest, SetupSourceRoot)
{
    CMakeModel model;

    EXPECT_TRUE(model.SetupSourceRoot(TEST_DATA_DIR, BuildDir));

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 21 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ(ARGN, model.GetVariable("ARGN"));
    EXPECT_EQ(ARGV, model.GetVariable("ARGV"));
    EXPECT_EQ(CMAKE_BUILD_TYPE, model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ(CMAKE_HOST_SYSTEM_NAME, model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ(CMAKE_HOST_WIN32, model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ(ISWIN32, model.GetVariable("WIN32"));
    EXPECT_EQ(CMAKE_SOURCE_DIR, model.GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(CMAKE_BINARY_DIR, model.GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, model.GetVariable(VarCurrentSourceDirectory));
    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, model.GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_DIR, model.GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_FILE, model.GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(CMAKE_PARENT_LIST_FILE, model.GetVariable(VarParentScriptPath));
}

TEST_F(CMakeModelTest, SetupCMakePath)
{
    CMakeModel model;
        
    EXPECT_TRUE(model.SetupSourceRoot(TEST_DATA_DIR, BuildDir));
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 30 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ(ARGN, model.GetVariable("ARGN"));
    EXPECT_EQ(ARGV, model.GetVariable("ARGV"));
    EXPECT_EQ(CMAKE_BUILD_TYPE, model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ(CMAKE_HOST_SYSTEM_NAME, model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ(CMAKE_HOST_WIN32, model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ(ISWIN32, model.GetVariable("WIN32"));
    EXPECT_EQ(CMAKE_SOURCE_DIR, model.GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(CMAKE_BINARY_DIR, model.GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, model.GetVariable(VarCurrentSourceDirectory));
    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, model.GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_DIR, model.GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_FILE, model.GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(CMAKE_PARENT_LIST_FILE, model.GetVariable(VarParentScriptPath));
    EXPECT_EQ(CMAKE_COMMAND, model.GetVariable(VarCMakeExePath));
    EXPECT_EQ(CMAKE_CPACK_COMMAND, model.GetVariable(VarCPackExePath));
    EXPECT_EQ(CMAKE_CTEST_COMMAND, model.GetVariable(VarCTestExePath));
    EXPECT_EQ(CMAKE_MAJOR_VERSION, model.GetVariable(VarCMakeVersionMajor));
    EXPECT_EQ(CMAKE_MINOR_VERSION, model.GetVariable(VarCMakeVersionMinor));
    EXPECT_EQ(CMAKE_PATCH_VERSION, model.GetVariable(VarCMakeVersionPatch));
    EXPECT_EQ(CMAKE_ROOT, model.GetVariable(VarCMakeRootPath));
    EXPECT_EQ(CMAKE_TWEAK_VERSION, model.GetVariable(VarCMakeVersionTweak));
    EXPECT_EQ(CMAKE_VERSION, model.GetVariable(VarCMakeVersion));
}

TEST_F(CMakeModelTest, SetupNinjaPath)
{
    CMakeModel model;

    EXPECT_TRUE(model.SetupSourceRoot(TEST_DATA_DIR, BuildDir));
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 32 }, model.GetVariables().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ(ARGN, model.GetVariable("ARGN"));
    EXPECT_EQ(ARGV, model.GetVariable("ARGV"));
    EXPECT_EQ(CMAKE_BUILD_TYPE, model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ(CMAKE_HOST_SYSTEM_NAME, model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ(CMAKE_HOST_WIN32, model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ(ISWIN32, model.GetVariable("WIN32"));
    EXPECT_EQ(CMAKE_SOURCE_DIR, model.GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(CMAKE_BINARY_DIR, model.GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, model.GetVariable(VarCurrentSourceDirectory));
    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, model.GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_DIR, model.GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_FILE, model.GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(CMAKE_PARENT_LIST_FILE, model.GetVariable(VarParentScriptPath));
    EXPECT_EQ(CMAKE_COMMAND, model.GetVariable(VarCMakeExePath));
    EXPECT_EQ(CMAKE_CPACK_COMMAND, model.GetVariable(VarCPackExePath));
    EXPECT_EQ(CMAKE_CTEST_COMMAND, model.GetVariable(VarCTestExePath));
    EXPECT_EQ(CMAKE_MAJOR_VERSION, model.GetVariable(VarCMakeVersionMajor));
    EXPECT_EQ(CMAKE_MINOR_VERSION, model.GetVariable(VarCMakeVersionMinor));
    EXPECT_EQ(CMAKE_PATCH_VERSION, model.GetVariable(VarCMakeVersionPatch));
    EXPECT_EQ(CMAKE_ROOT, model.GetVariable(VarCMakeRootPath));
    EXPECT_EQ(CMAKE_TWEAK_VERSION, model.GetVariable(VarCMakeVersionTweak));
    EXPECT_EQ(CMAKE_VERSION, model.GetVariable(VarCMakeVersion));
    EXPECT_EQ(CMAKE_GENERATOR, model.GetVariable(VarCMakeGenerator));
    EXPECT_EQ(CMAKE_MAKE_PROGRAM, model.GetVariable(VarMakeProgramPath));
}

TEST_F(CMakeModelTest, SetupCMakeFile)
{
    CMakeModel model;

    EXPECT_TRUE(model.SetupSourceRoot(TEST_DATA_DIR, BuildDir));
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.SetupCMakeFile("code"));

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 32 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 2 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ(ARGN, model.GetVariable("ARGN"));
    EXPECT_EQ(ARGV, model.GetVariable("ARGV"));
    EXPECT_EQ(CMAKE_BUILD_TYPE, model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ(CMAKE_HOST_SYSTEM_NAME, model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ(CMAKE_HOST_WIN32, model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ(ISWIN32, model.GetVariable("WIN32"));
    EXPECT_EQ(CMAKE_SOURCE_DIR, model.GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(CMAKE_BINARY_DIR, model.GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR) / "code", model.GetVariable(VarCurrentSourceDirectory));
    EXPECT_EQ(std::filesystem::path(CMAKE_CURRENT_BINARY_DIR) / "code", model.GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(std::filesystem::path(CMAKE_CURRENT_LIST_DIR) / "code", model.GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(std::filesystem::path(CMAKE_CURRENT_LIST_DIR) / "code" / "CMakeLists.txt", model.GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(CMAKE_PARENT_LIST_FILE, model.GetVariable(VarParentScriptPath));
    EXPECT_EQ(CMAKE_COMMAND, model.GetVariable(VarCMakeExePath));
    EXPECT_EQ(CMAKE_CPACK_COMMAND, model.GetVariable(VarCPackExePath));
    EXPECT_EQ(CMAKE_CTEST_COMMAND, model.GetVariable(VarCTestExePath));
    EXPECT_EQ(CMAKE_MAJOR_VERSION, model.GetVariable(VarCMakeVersionMajor));
    EXPECT_EQ(CMAKE_MINOR_VERSION, model.GetVariable(VarCMakeVersionMinor));
    EXPECT_EQ(CMAKE_PATCH_VERSION, model.GetVariable(VarCMakeVersionPatch));
    EXPECT_EQ(CMAKE_ROOT, model.GetVariable(VarCMakeRootPath));
    EXPECT_EQ(CMAKE_TWEAK_VERSION, model.GetVariable(VarCMakeVersionTweak));
    EXPECT_EQ(CMAKE_VERSION, model.GetVariable(VarCMakeVersion));
    EXPECT_EQ(CMAKE_GENERATOR, model.GetVariable(VarCMakeGenerator));
    EXPECT_EQ(CMAKE_MAKE_PROGRAM, model.GetVariable(VarMakeProgramPath));
    EXPECT_NOT_NULL(model.FindDirectory(TEST_DATA_DIR));
    EXPECT_NOT_NULL(model.FindDirectory(std::filesystem::path(TEST_DATA_DIR) / "code"));
}

TEST_F(CMakeModelTest, GetCacheVariables)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variableName{ "CMAKE_BUILD_TYPE" };
    std::string variableType{ "STRING" };
    std::string variableValue{ "Debug" };
    model.SetVariable(variableName, variableValue, VariableAttribute::Cache);

    auto variables = model.GetCacheVariables();
    EXPECT_EQ(size_t{ 1 }, variables.size());
    ASSERT_TRUE(variables.find(variableName) != variables.end());
    EXPECT_EQ(variableType, variables.find(variableName)->second->Type());
    EXPECT_EQ(variableValue, variables.find(variableName)->second->Value());
}

TEST_F(CMakeModelTest, GetVariables)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    std::string variableName{ "CMAKE_BUILD_TYPE" };
    std::string variableValue{ "Debug" };
    EXPECT_TRUE(model.IsSourceRootSet());
    auto variables = model.GetVariables();
    EXPECT_EQ(size_t{ 32 }, variables.size());
    ASSERT_TRUE(variables.find(variableName) != variables.end());
    EXPECT_EQ(variableValue, variables.find(variableName)->second->Value());
}

TEST_F(CMakeModelTest, GetEnvironmentVariables)
{
    CMakeModel model;

    auto const& variables = model.GetEnvironmentVariables();
    EXPECT_EQ(size_t{ 0 }, variables.size());

    std::string variableName{ "MYDIR" };
    std::string variableValue{ "PATH" };
    model.SetEnvironmentVariable(variableName, variableValue);

    EXPECT_EQ(size_t{ 1 }, variables.size());
    ASSERT_TRUE(variables.find(variableName) != variables.end());
    EXPECT_EQ(variableValue, variables.find(variableName)->second->Value());
}

TEST_F(CMakeModelTest, GetCacheVariable)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variableName{ "CMAKE_BUILD_TYPE" };
    std::string variableValue{ "Debug" };
    model.SetVariable(variableName, variableValue, VariableAttribute::Cache);

    EXPECT_EQ(variableValue, model.GetCacheVariable(variableName));
    EXPECT_NOT_NULL(model.FindCacheVariable(variableName));
}

TEST_F(CMakeModelTest, GetVariable)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variableName{ "CMAKE_BUILD_TYPE" };
    std::string variableValue{ "Debug" };
    auto variables = model.GetVariables();
    EXPECT_EQ(variableValue, model.GetVariable(variableName));
    EXPECT_NOT_NULL(model.FindVariable(variableName));
}

TEST_F(CMakeModelTest, GetEnvironmentVariable)
{
    CMakeModel model;

    std::string variableName{ "MYDIR" };
    std::string variableValue{ "PATH" };
    EXPECT_EQ("", model.GetEnvironmentVariable(variableName));
    ASSERT_NULL(model.FindEnvironmentVariable("DUMMY"));

    model.SetEnvironmentVariable(variableName, variableValue);
    EXPECT_EQ(variableValue, model.GetEnvironmentVariable(variableName));
    EXPECT_NOT_NULL(model.FindEnvironmentVariable(variableName));
}

TEST_F(CMakeModelTest, SetVariableCache)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variable1Name{ "CMAKE_BUILD_TYPE" };
    std::string variable1Type{ "STRING" };
    std::string variable1Value{ "Debug" };
    std::string variable2Name{ "DUMMY" };
    std::string variable2Type{ "BOOL" };
    std::string variable2Value{ "OFF" };
    std::string variable3Name{ "DUMMY2" };
    std::string variable3Type{ "STRING" };
    std::string variable3Value{ "TEXT" };
    model.SetVariable(variable1Name, variable1Value, VariableAttribute::Cache);

    model.SetVariable(variable1Name, "Release", VariableAttribute::Cache, variable1Type);
    EXPECT_EQ(variable1Value, model.GetCacheVariable(variable1Name));
    model.SetVariable(variable2Name, variable2Value, VariableAttribute::Cache, variable2Type);
    model.SetVariable(variable3Name, variable3Value, VariableAttribute::Cache);
    ASSERT_NOT_NULL(model.FindCacheVariable(variable1Name));
    ASSERT_NOT_NULL(model.FindCacheVariable(variable2Name));
    EXPECT_EQ(variable2Type, model.FindCacheVariable(variable2Name)->Type());
    EXPECT_EQ(variable2Value, model.FindCacheVariable(variable2Name)->Value());
    ASSERT_NOT_NULL(model.FindCacheVariable(variable3Name));
    EXPECT_EQ(variable3Type, model.FindCacheVariable(variable3Name)->Type());
    EXPECT_EQ(variable3Value, model.FindCacheVariable(variable3Name)->Value());
}

TEST_F(CMakeModelTest, SetVariableCacheForce)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variable1Name{ "CMAKE_BUILD_TYPE" };
    std::string variable1Type{ "STRING" };
    std::string variable1Value{ "Debug" };
    std::string variable1ValueNew{ "Release" };
    std::string variable2Name{ "DUMMY" };
    std::string variable2Type{ "BOOL" };
    std::string variable2TypeNew{ "STRING" };
    std::string variable2Value{ "OFF" };
    std::string variable2ValueNew{ "More text" };
    std::string variable3Name{ "DUMMY2" };
    std::string variable3Type{ "STRING" };
    std::string variable3Value{ "Text" };
    model.SetVariable(variable1Name, variable1Value, VariableAttribute::Cache);

    model.SetVariable(variable1Name, variable1ValueNew, static_cast<VariableAttribute>(VariableAttribute::Cache | VariableAttribute::Force));
    ASSERT_NOT_NULL(model.FindCacheVariable(variable1Name));
    EXPECT_EQ(variable1Type, model.FindCacheVariable(variable1Name)->Type());
    EXPECT_EQ(variable1ValueNew, model.FindCacheVariable(variable1Name)->Value());
    model.SetVariable(variable2Name, variable2Value, VariableAttribute::Cache | VariableAttribute::Force, variable2Type);
    model.SetVariable(variable3Name, variable3Value, VariableAttribute::Cache | VariableAttribute::Force);
    ASSERT_NOT_NULL(model.FindCacheVariable(variable2Name));
    ASSERT_NULL(model.FindVariable(variable2Name));
    EXPECT_EQ(variable2Type, model.FindCacheVariable(variable2Name)->Type());
    EXPECT_EQ(variable2Value, model.FindCacheVariable(variable2Name)->Value());
    ASSERT_NOT_NULL(model.FindCacheVariable(variable3Name));
    ASSERT_NULL(model.FindVariable(variable3Name));
    EXPECT_EQ(variable3Type, model.FindCacheVariable(variable3Name)->Type());
    EXPECT_EQ(variable3Value, model.FindCacheVariable(variable3Name)->Value());
    model.SetVariable(variable2Name, variable2ValueNew, VariableAttribute::Cache | VariableAttribute::Force, variable2TypeNew);
    ASSERT_NOT_NULL(model.FindCacheVariable(variable2Name));
    EXPECT_EQ(variable2TypeNew, model.FindCacheVariable(variable2Name)->Type());
    EXPECT_EQ(variable2ValueNew, model.FindCacheVariable(variable2Name)->Value());
}

TEST_F(CMakeModelTest, SetVariable)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variable1Name{ "DUMMY" };
    std::string variable1Value{ "OFF" };
    std::string variable2Name{ "DUMMY2" };
    std::string variable2Value{ "Text" };
    model.SetVariable(variable1Name, variable1Value);

    model.SetVariable(variable2Name, variable2Value, VariableAttribute::None);
    ASSERT_NULL(model.FindCacheVariable(variable1Name));
    ASSERT_NOT_NULL(model.FindVariable(variable1Name));
    EXPECT_EQ(variable1Value, model.FindVariable(variable1Name)->Value());
    ASSERT_NULL(model.FindCacheVariable(variable2Name));
    ASSERT_NOT_NULL(model.FindVariable(variable2Name));
    EXPECT_EQ(variable2Value, model.FindVariable(variable2Name)->Value());
}

TEST_F(CMakeModelTest, SetEnvironmentVariable)
{
    CMakeModel model;

    std::string variableName{ "MYDIR" };
    std::string variableValue{ "PATH" };
    model.SetEnvironmentVariable(variableName, variableValue);
    ASSERT_NOT_NULL(model.FindEnvironmentVariable(variableName));
    EXPECT_EQ(variableValue, model.FindEnvironmentVariable(variableName)->Value());
}

TEST_F(CMakeModelTest, UnsetVariableCache)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variableName{ "CMAKE_BUILD_TYPE" };
    std::string variableValue{ "Debug" };
    model.SetVariable(variableName, variableValue, VariableAttribute::Cache);

    EXPECT_EQ(variableValue, model.GetCacheVariable(variableName));
    EXPECT_NOT_NULL(model.FindCacheVariable(variableName));
    model.UnsetVariable(variableName, VariableAttribute::Cache);
    EXPECT_NULL(model.FindCacheVariable(variableName));
    EXPECT_EQ("", model.GetCacheVariable(variableName));
}

TEST_F(CMakeModelTest, UnsetVariable)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    std::string variableName{ "DUMMY" };
    std::string variableValue{ "OFF" };
    model.SetVariable(variableName, variableValue);

    ASSERT_NOT_NULL(model.FindVariable(variableName));
    EXPECT_EQ(variableValue, model.GetVariable(variableName));
    model.UnsetVariable(variableName);
    ASSERT_NULL(model.FindVariable(variableName));
    EXPECT_EQ("", model.GetVariable(variableName));
}

TEST_F(CMakeModelTest, UnsetEnvironmentVariable)
{
    CMakeModel model;

    std::string variableName{ "MYDIR" };
    std::string variableValue{ "PATH" };
    model.SetEnvironmentVariable(variableName, variableValue);

    ASSERT_NOT_NULL(model.FindEnvironmentVariable(variableName));
    EXPECT_EQ(variableValue, model.GetEnvironmentVariable(variableName));
    model.UnsetEnvironmentVariable(variableName);
    ASSERT_NULL(model.FindEnvironmentVariable(variableName));
    EXPECT_EQ("", model.GetEnvironmentVariable(variableName));
}

TEST_F(CMakeModelTest, GetProjects)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(model.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(model.AddProject(project2));

    EXPECT_EQ(size_t{ 2 }, model.GetProjects().size());
    auto const& subProjects = model.GetSubProjects(project1);
    EXPECT_EQ(size_t{ 1 }, subProjects.size());
    EXPECT_NE(subProjects.end(), subProjects.find(projectName2));
    EXPECT_EQ(project1, model.GetParentProject(project2));
    EXPECT_NULL(model.GetParentProject(project1));
    EXPECT_EQ(project1, model.GetMainProject());
}

TEST_F(CMakeModelTest, GetProject)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(model.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(model.AddProject(project2));

    EXPECT_EQ(project1, model.GetProject(projectName1));
    EXPECT_EQ(project2, model.GetProject(projectName2));
}

TEST_F(CMakeModelTest, AddProject)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR, BuildDir);
    model.SetupCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());
    EXPECT_TRUE(model.IsSourceRootSet());

    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(model.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(model.AddProject(project2));

    EXPECT_FALSE(model.AddProject(project1));

    EXPECT_EQ(size_t{ 2 }, model.GetProjects().size());
    EXPECT_EQ(project1, model.GetProject(projectName1));
    EXPECT_EQ(project2, model.GetProject(projectName2));
    auto const& subProjects = model.GetSubProjects(project1);
    EXPECT_EQ(size_t{ 1 }, subProjects.size());
    EXPECT_NE(subProjects.end(), subProjects.find(projectName2));
    EXPECT_EQ(project1, model.GetParentProject(project2));
    EXPECT_NULL(model.GetParentProject(project1));

    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, model.GetVariable("PROJECT_BINARY_DIR"));
    EXPECT_EQ("", model.GetVariable("PROJECT_HOMEPAGE_URL"));
    EXPECT_EQ(projectName2, model.GetVariable("PROJECT_NAME"));
    EXPECT_EQ(description2, model.GetVariable("PROJECT_DESCRIPTION"));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, model.GetVariable("PROJECT_SOURCE_DIR"));
    EXPECT_EQ(version2, model.GetVariable("PROJECT_VERSION"));
    EXPECT_EQ("1", model.GetVariable("PROJECT_VERSION_MAJOR"));
    EXPECT_EQ("2", model.GetVariable("PROJECT_VERSION_MINOR"));
    EXPECT_EQ("3", model.GetVariable("PROJECT_VERSION_PATCH"));
    EXPECT_EQ("5", model.GetVariable("PROJECT_VERSION_TWEAK"));
}

TEST_F(CMakeModelTest, GetDirectories)
{
    CMakeModel model;

    EXPECT_EQ(size_t{ 0 }, model.GetDirectories().size());

    auto directoryPath = TEST_DATA_DIR;
    model.SetupSourceRoot(directoryPath, BuildDir);
    model.SetupCMakeFile(std::string(directoryPath) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
}

TEST_F(CMakeModelTest, FindDirectory)
{
    CMakeModel model;

    std::filesystem::path directoryPath = TEST_DATA_DIR;
    EXPECT_NULL(model.FindDirectory(directoryPath));

    model.SetupSourceRoot(directoryPath, BuildDir);
    model.SetupCMakeFile((directoryPath / "CMakeLists.txt").string());
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_NOT_NULL(model.FindDirectory(directoryPath));
}

TEST_F(CMakeModelTest, GetDirectory)
{
    CMakeModel model;

    std::filesystem::path directoryPath = TEST_DATA_DIR;
    EXPECT_EQ("", model.GetDirectory(directoryPath));

    model.SetupSourceRoot(directoryPath, BuildDir);
    model.SetupCMakeFile((directoryPath / "CMakeLists.txt").string());
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_EQ(directoryPath, model.GetDirectory(directoryPath));
}

TEST_F(CMakeModelTest, AddDirectory)
{
    CMakeModel model;

    std::filesystem::path sourceDirectoryPath = TEST_DATA_DIR;
    std::filesystem::path binaryDirectoryPath = std::filesystem::path(TEST_DATA_DIR) / "cmake-bin";
    EXPECT_EQ("", model.GetDirectory(sourceDirectoryPath));

    auto directory1 = std::make_shared<Directory>(sourceDirectoryPath, binaryDirectoryPath);
    EXPECT_NULL(model.FindDirectory(sourceDirectoryPath));

    EXPECT_TRUE(model.AddDirectory(directory1));

    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(sourceDirectoryPath));

    std::filesystem::path sourceSubDirectoryPath = "subDir";
    std::filesystem::path binarySubDirectoryPath = "subDir";
    auto directory2 = std::make_shared<Directory>(sourceSubDirectoryPath, binarySubDirectoryPath, directory1);
    EXPECT_NULL(model.FindDirectory(sourceSubDirectoryPath));

    EXPECT_TRUE(model.AddDirectory(directory2));

    EXPECT_EQ(size_t{ 2 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(sourceSubDirectoryPath));

    EXPECT_FALSE(model.AddDirectory(directory1));

    EXPECT_EQ(size_t{ 2 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(sourceDirectoryPath));

    auto const& subDirectories = model.GetSubDirectories(directory1);
    EXPECT_EQ(size_t{ 1 }, subDirectories.size());
    EXPECT_NE(subDirectories.end(), subDirectories.find(sourceSubDirectoryPath));
    EXPECT_EQ(directory1, model.GetParentDirectory(directory2));
    EXPECT_NULL(model.GetParentDirectory(directory1));
    EXPECT_EQ(directory1, model.GetRootDirectory());
}

TEST_F(CMakeModelTest, AddMessage)
{
    CMakeModel model;

    std::string messageMode{ "STATUS" };
    std::string message{ "Hello World" };
    model.AddMessage(messageMode, message);
    EXPECT_EQ(size_t{ 1 }, m_lineWriter.GetResult().size());
    EXPECT_TRUE(utility::VerifyMatch(m_lineWriter.GetResult()[0], utility::FormatString("Info \\|CMakeModel.cpp:\\d+\\|AddMessage\\|Message\\({} {}\\)", messageMode, message)));
}

} // namespace cmake_parser
