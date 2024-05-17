#include "cmake-parser/ScriptParser.h"

#include "test-platform/GoogleTest.h"
#include <filesystem>
#include <fstream>
#include "utility/CommandLine.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"
#include "cmake-parser/CMakeParser.h"

using namespace tracing;

namespace cmake_parser {

class ScriptParserTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    ScriptParserTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
        Tracing::SetDefaultTraceFilter(TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information | TraceCategory::Data | TraceCategory::Debug);
    }
    void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

static const std::filesystem::path TOOLCHAIN_PATH{ "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64" };
static const std::filesystem::path CMAKE_BINARY_DIR{ std::filesystem::path(TEST_DATA_DIR) / "cmake-x64-Debug" };
static const std::string CMAKE_BUILD_TYPE{ "Debug" };
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
static const std::filesystem::path CMAKE_PARENT_LIST_FILE{ CMAKE_CURRENT_LIST_FILE };
static const std::filesystem::path CMAKE_SOURCE_DIR{ TEST_DATA_DIR };

TEST_F(ScriptParserTest, EmptyStream)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "empty" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ(size_t{ 32 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));
    
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetDirectories().size());
    auto directory = parser.GetModel().FindDirectory(rootDirectory);
    ASSERT_NOT_NULL(directory);
    EXPECT_EQ(rootDirectory, directory->SourcePath());
    EXPECT_NULL(directory->Parent());
}

TEST_F(ScriptParserTest, MinimalCMakeFile)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "minimal" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ(size_t{ 33 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));

    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetDirectories().size());
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory));
}

TEST_F(ScriptParserTest, MinimalCMakeProject)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "minimal_project" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ(size_t{ 56 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));

    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetProjects().size());
    std::string projectName{ "CPP-Parser" };
    auto project = parser.GetModel().GetProject(projectName);
    ASSERT_NOT_NULL(project);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("4.3.2.1", project->Version());
    EXPECT_EQ("This is the project for cpp-parser", project->Description());
    EXPECT_EQ("CXX", project->Languages());
    EXPECT_NULL(project->Parent());
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarMainProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarMainProjectHomePageURL));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarProjectBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarProjectHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(VarProjectIsTopLevel));
    EXPECT_EQ(project->Name(), parser.GetModel().GetVariable(VarProjectName));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarProjectSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(VarProjectVersion));
    EXPECT_EQ("4", parser.GetModel().GetVariable(VarProjectVersionMajor));
    EXPECT_EQ("3", parser.GetModel().GetVariable(VarProjectVersionMinor));
    EXPECT_EQ("2", parser.GetModel().GetVariable(VarProjectVersionPatch));
    EXPECT_EQ("1", parser.GetModel().GetVariable(VarProjectVersionTweak));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(projectName + VarSuffixBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(projectName + VarSuffixDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(projectName + VarSuffixHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(projectName + VarSuffixIsTopLevel));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(projectName + VarSuffixSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(projectName + VarSuffixVersion));
    EXPECT_EQ("4", parser.GetModel().GetVariable(projectName + VarSuffixVersionMajor));
    EXPECT_EQ("3", parser.GetModel().GetVariable(projectName + VarSuffixVersionMinor));
    EXPECT_EQ("2", parser.GetModel().GetVariable(projectName + VarSuffixVersionPatch));
    EXPECT_EQ("1", parser.GetModel().GetVariable(projectName + VarSuffixVersionTweak));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetDirectories().size());
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory));
}

TEST_F(ScriptParserTest, MinimalCMakeProjectWithMessage)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "minimal_project_message" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ(size_t{ 56 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));

    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetProjects().size());
    std::string projectName{ "my-project" };
    auto project = parser.GetModel().GetProject(projectName);
    ASSERT_NOT_NULL(project);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("", project->Version());
    EXPECT_EQ("My project", project->Description());
    EXPECT_EQ("", project->Languages());
    EXPECT_NULL(project->Parent());
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarMainProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarMainProjectHomePageURL));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarProjectBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarProjectHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(VarProjectIsTopLevel));
    EXPECT_EQ(project->Name(), parser.GetModel().GetVariable(VarProjectName));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarProjectSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(VarProjectVersion));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionMajor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionMinor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionPatch));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionTweak));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(projectName + VarSuffixBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(projectName + VarSuffixDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(projectName + VarSuffixHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(projectName + VarSuffixIsTopLevel));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(projectName + VarSuffixSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(projectName + VarSuffixVersion));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionMajor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionMinor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionPatch));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionTweak));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetDirectories().size());
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory));
}

TEST_F(ScriptParserTest, MinimalCMakeProjectWithSet)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "minimal_project_set" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ("Hello world", parser.GetModel().GetEnvironmentVariable("myvar"));
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ(size_t{ 72 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));

    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    std::string projectName{ "my-project" };
    std::string definitions{ "_DEF_" };
    std::string compilerOptions{ "/wd0000" };
    std::string linkerOptions{ "/lx1234" };
    std::string linkerLibraries{ "mylib" };
    std::string dependencies{ "mydep" };
    EXPECT_EQ(definitions, parser.GetModel().GetVariable("COMPILER_DEFINITIONS"));
    EXPECT_EQ(compilerOptions, parser.GetModel().GetVariable("COMPILER_OPTIONS_CXX"));
    EXPECT_EQ(linkerOptions, parser.GetModel().GetVariable("LINKER_OPTIONS"));
    EXPECT_EQ(linkerLibraries, parser.GetModel().GetVariable("LINKER_LIBRARIES"));

    EXPECT_EQ("Windows", parser.GetModel().GetVariable("PLATFORM_NAME"));
    EXPECT_EQ((rootDirectory / "output" / parser.GetModel().GetVariable("PLATFORM_NAME")).generic_string(), parser.GetModel().GetVariable("OUTPUT_BASE_DIR"));
    EXPECT_EQ("Debug", parser.GetModel().GetVariable("CONFIG_DIR"));
    EXPECT_EQ(projectName, parser.GetModel().GetVariable("TARGET_NAME"));
    EXPECT_EQ("PACKAGE_NAME=\\" + projectName + "\\;_DEF_", parser.GetModel().GetVariable("PROJECT_COMPILER_DEFINITIONS_PRIVATE"));
    EXPECT_EQ("/wd0000;/wd4191;/wd4706", parser.GetModel().GetVariable("PROJECT_COMPILER_OPTIONS_PRIVATE"));
    EXPECT_EQ((rootDirectory / "include").generic_string() + ";" + (rootDirectory / "src").generic_string(), parser.GetModel().GetVariable("PROJECT_INCLUDE_DIRS_PRIVATE"));
    EXPECT_EQ(linkerOptions, parser.GetModel().GetVariable("PROJECT_LINK_OPTIONS"));
    EXPECT_EQ(dependencies, parser.GetModel().GetVariable("PROJECT_DEPENDENCIES"));
    EXPECT_EQ(linkerLibraries + ";" + dependencies, parser.GetModel().GetVariable("PROJECT_LIBS"));
    EXPECT_EQ(rootDirectory / "src" / "main.cpp", parser.GetModel().GetVariable("PROJECT_SOURCES"));
    EXPECT_EQ(rootDirectory / "include" / "getopt.h", parser.GetModel().GetVariable("PROJECT_INCLUDES_PRIVATE"));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetProjects().size());
    auto project = parser.GetModel().GetProject(projectName);
    ASSERT_NOT_NULL(project);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("", project->Version());
    EXPECT_EQ("My project", project->Description());
    EXPECT_EQ("", project->Languages());
    EXPECT_NULL(project->Parent());
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarMainProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarMainProjectHomePageURL));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarProjectBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarProjectDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarProjectHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(VarProjectIsTopLevel));
    EXPECT_EQ(project->Name(), parser.GetModel().GetVariable(VarProjectName));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarProjectSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(VarProjectVersion));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionMajor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionMinor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionPatch));
    EXPECT_EQ("0", parser.GetModel().GetVariable(VarProjectVersionTweak));

    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(projectName + VarSuffixBinaryDir));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(projectName + VarSuffixDescription));
    EXPECT_EQ("", parser.GetModel().GetVariable(projectName + VarSuffixHomepageURL));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable(projectName + VarSuffixIsTopLevel));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(projectName + VarSuffixSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(projectName + VarSuffixVersion));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionMajor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionMinor));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionPatch));
    EXPECT_EQ("0", parser.GetModel().GetVariable(projectName + VarSuffixVersionTweak));

    EXPECT_EQ(size_t{ 1 }, parser.GetModel().GetDirectories().size());
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory));
}

TEST_F(ScriptParserTest, SimpleProject)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) / "simple_project" };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());

    std::string actualOutput = parser.Serialize();
    auto actualOutputPath = rootDirectory / "parser_out.json";
    auto actualParserOutput = std::ofstream(actualOutputPath, std::ios_base::out);
    actualParserOutput.write(actualOutput.c_str(), static_cast<std::streamsize>(actualOutput.size()));
    actualParserOutput.close();

    std::string expectedOutput;
    auto expectedOutputPath = rootDirectory / "parser.json";
    auto expectedParserOutput = std::ifstream(expectedOutputPath, std::ios_base::in);
    const size_t BufferSize = 4096;
    char buffer[BufferSize];
    while (expectedParserOutput.read(buffer, BufferSize))
    {
        expectedOutput += std::string(buffer, static_cast<std::size_t>(expectedParserOutput.gcount()));
    }
    expectedOutput += std::string(buffer, static_cast<std::size_t>(expectedParserOutput.gcount()));
    expectedParserOutput.close();
    EXPECT_EQ(expectedOutput, actualOutput);
}

TEST_F(ScriptParserTest, CMakeProject)
{
    std::filesystem::path rootDirectory{ std::filesystem::path(TEST_DATA_DIR) };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser topLevelParser(rootDirectory, buildDir, stream);
    ScriptParser parser(topLevelParser.GetModel(), rootDirectory, stream);

    EXPECT_TRUE(parser.Parse());
    const std::filesystem::path serializationTestSourceDir{ rootDirectory / "code" / "libraries" / "serialization" / "test" };
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
    EXPECT_EQ("", parser.GetModel().GetEnvironmentVariable("myvar"));
    EXPECT_EQ(size_t{ 2 }, parser.GetModel().GetCacheVariables().size());
    EXPECT_EQ((rootDirectory / "cmake").generic_string(), parser.GetModel().GetCacheVariable("SCRIPTS_DIR"));
    EXPECT_EQ("STRING", parser.GetModel().FindCacheVariable("SCRIPTS_DIR")->Type());
    EXPECT_EQ("CMake scripts path", parser.GetModel().FindCacheVariable("SCRIPTS_DIR")->Description());
    EXPECT_EQ(
        (serializationTestSourceDir / "src" / "main.cpp").generic_string() + ";" +
        (serializationTestSourceDir / "src" / "BidirectionalMapTest.cpp").generic_string() + ";" +
        (serializationTestSourceDir / "src" / "EnumSerializationTest.cpp").generic_string() + ";" +
        (serializationTestSourceDir / "src" / "MapUtilitiesTest.cpp").generic_string() + ";" +
        (serializationTestSourceDir / "src" / "SerializationTest.cpp").generic_string(),
        parser.GetModel().GetCacheVariable("PROJECT_SOURCES_serialization-test"));
    ASSERT_NOT_NULL(parser.GetModel().FindCacheVariable("PROJECT_SOURCES_serialization-test"));
    EXPECT_EQ("STRING", parser.GetModel().FindCacheVariable("PROJECT_SOURCES_serialization-test")->Type());
    EXPECT_EQ("serialization-test", parser.GetModel().FindCacheVariable("PROJECT_SOURCES_serialization-test")->Description());
    EXPECT_EQ(size_t{ 85 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));

    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(rootDirectory / buildDir, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(rootDirectory / CMakeScriptFileName, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(rootDirectory, parser.GetModel().GetVariable(VarCurrentSourceDirectory));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    EXPECT_EQ(size_t{ 4 }, parser.GetModel().GetProjects().size());
    std::string projectName{ "CPP-Parser" };
    std::string projectDescription{ "This is the project for cpp-parser" };
    auto mainProject = parser.GetModel().GetProject(projectName);
    EXPECT_EQ(projectName, mainProject->Name());
    EXPECT_EQ("1.2.3.4", mainProject->Version());
    EXPECT_EQ(projectDescription, mainProject->Description());
    EXPECT_EQ("", mainProject->Languages());
    EXPECT_EQ("", mainProject->HomePageURL());
    EXPECT_NULL(mainProject->Parent());

    projectName = { "serializer" };
    projectDescription = { "CPP parser" };
    auto project = parser.GetModel().GetProject(projectName);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("1.2.3.4", project->Version());
    EXPECT_EQ(projectDescription, project->Description());
    EXPECT_EQ("CXX", project->Languages());
    EXPECT_EQ("", project->HomePageURL());
    EXPECT_EQ(mainProject, project->Parent());

    projectName = { "serialization" };
    projectDescription = { "Serialization library" };
    project = parser.GetModel().GetProject(projectName);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("1.2.3.4", project->Version());
    EXPECT_EQ(projectDescription, project->Description());
    EXPECT_EQ("CXX", project->Languages());
    EXPECT_EQ("", project->HomePageURL());
    EXPECT_EQ(mainProject, project->Parent());

    projectName = { "serialization-test" };
    projectDescription = { "Serialization Library Test" };
    project = parser.GetModel().GetProject(projectName);
    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ("1.2.3.4", project->Version());
    EXPECT_EQ(projectDescription, project->Description());
    EXPECT_EQ("CXX", project->Languages());
    EXPECT_EQ("", project->HomePageURL());
    EXPECT_EQ(mainProject, project->Parent());

    projectName = { "CPP-Parser" };
    project = parser.GetModel().GetProject(projectName);
    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, parser.GetModel().GetVariable(VarProjectBinaryDir));
    EXPECT_EQ("", parser.GetModel().GetVariable(VarProjectHomepageURL));
    EXPECT_EQ(project->Name(), parser.GetModel().GetVariable(VarProjectName));
    EXPECT_EQ(project->Description(), parser.GetModel().GetVariable(VarProjectDescription));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, parser.GetModel().GetVariable(VarProjectSourceDirectory));
    EXPECT_EQ(project->Version(), parser.GetModel().GetVariable(VarProjectVersion));
    EXPECT_EQ("1", parser.GetModel().GetVariable(VarProjectVersionMajor));
    EXPECT_EQ("2", parser.GetModel().GetVariable(VarProjectVersionMinor));
    EXPECT_EQ("3", parser.GetModel().GetVariable(VarProjectVersionPatch));
    EXPECT_EQ("4", parser.GetModel().GetVariable(VarProjectVersionTweak));

    const std::string platformName{ "windows" };
    EXPECT_EQ("1.2.3.4", parser.GetModel().GetVariable("MSI_NUMBER"));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable("PLATFORM_WINDOWS"));
    EXPECT_EQ(platformName, parser.GetModel().GetVariable("PLATFORM_NAME"));
    EXPECT_EQ("ON", parser.GetModel().GetVariable("CMAKE_EXPORT_COMPILE_COMMANDS"));
    EXPECT_EQ("OFF", parser.GetModel().GetVariable("CMAKE_COLOR_MAKEFILE"));
    EXPECT_EQ("ON", parser.GetModel().GetVariable("CMAKE_BUILD_WITH_INSTALL_RPATH"));
    EXPECT_EQ("OFF", parser.GetModel().GetVariable("CMAKE_VERBOSE_MAKEFILE"));
    EXPECT_EQ(rootDirectory / "output" / platformName, parser.GetModel().GetVariable("OUTPUT_BASE_DIR"));
    EXPECT_EQ(rootDirectory / "testdata", parser.GetModel().GetVariable("TEST_DATA_DIR"));
    EXPECT_EQ("CMAKE_CXX_FLAGS;CMAKE_CXX_FLAGS_DEBUG;CMAKE_CXX_FLAGS_RELEASE;CMAKE_C_FLAGS;CMAKE_C_FLAGS_DEBUG;CMAKE_C_FLAGS_RELEASE", parser.GetModel().GetVariable("CompilerFlags"));
    EXPECT_EQ("17", parser.GetModel().GetVariable("SUPPORTED_CPP_STANDARD"));
    EXPECT_EQ("/Wall;/wd4061;/wd4239;/wd4251;/wd4275;/wd4435;/wd4505;/wd4514;/wd4548;/wd4571;/wd4592;/wd4625;/wd4626;/wd4628;/wd4710;/wd4711;/wd4774;/wd4668;/wd5045;/wd4820;/wd5026;/wd5027;/WX-;/EHsc;/Gd;/GR;/sdl-;/Zc:wchar_t;/Zc:inline;/fp:precise;/bigobj;/std:c++17", 
        parser.GetModel().GetVariable("FLAGS_CXX"));
    EXPECT_EQ("/Od;/Gm-;/Zi;/RTC1;/MTd", parser.GetModel().GetVariable("FLAGS_CXX_DEBUG"));
    EXPECT_EQ("/Ox;/GL;/GS;/Gy;/Oi;/MT", parser.GetModel().GetVariable("FLAGS_CXX_RELEASE"));
    EXPECT_EQ("/O1;/GL;/GS;/Gy;/Oi;/MT", parser.GetModel().GetVariable("FLAGS_CXX_MINSIZEREL"));
    EXPECT_EQ("/O2;/GL;/GS;/Gy;/Oi;/Zi;/MT", parser.GetModel().GetVariable("FLAGS_CXX_RELWITHDEBINFO"));
    EXPECT_EQ("/Wall;/wd4061;/wd4251;/wd4275;/wd4505;/wd4514;/wd4548;/wd4571;/wd4625;/wd4626;/wd4710;/wd4774;/wd4820;/wd5026;/wd5027;/WX-;/EHsc;/Gd;/GR;/sdl-;/Zc:wchar_t;/Zc:inline;/fp:precise", parser.GetModel().GetVariable("FLAGS_C"));
    EXPECT_EQ("/Od;/Gm-;/ZI;/RTC1;/MTd", parser.GetModel().GetVariable("FLAGS_C_DEBUG"));
    EXPECT_EQ("/Ox;/GL;/GS;/Gy;/Oi;/MT", parser.GetModel().GetVariable("FLAGS_C_RELEASE"));
    EXPECT_EQ("/O1;/GL;/GS;/Gy;/Oi;/MT", parser.GetModel().GetVariable("FLAGS_C_MINSIZEREL"));
    EXPECT_EQ("/O2;/GL;/GS;/Gy;/Oi;/Zi;/MT", parser.GetModel().GetVariable("FLAGS_C_RELWITHDEBINFO"));
    EXPECT_EQ("_UNICODE;UNICODE;_CRT_SECURE_NO_WARNINGS;_SCL_SECURE_NO_WARNINGS;_WINSOCK_DEPRECATED_NO_WARNINGS;WINVER=0x0A00;_WIN32_WINNT=0x0A00;PLATFORM_WINDOWS;TEST_DATA_DIR=D:/Projects/CPPParser/testdata/testdata;_AMD64_;_X86_", 
        parser.GetModel().GetVariable("DEFINES"));
    EXPECT_EQ("_DEBUG", parser.GetModel().GetVariable("DEFINES_DEBUG"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_RELEASE"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_MINSIZEREL"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_RELWITHDEBINFO"));
    EXPECT_EQ("/ignore:4217", parser.GetModel().GetVariable("LINK_FLAGS"));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable("CMAKE_USE_WIN32_THREADS_INIT"));
    EXPECT_EQ("NEW", parser.GetModel().GetVariable("CMAKE_POLICY_DEFAULT_CMP0048"));

    // Todo: evaluate expressions

    EXPECT_EQ(size_t{ 7 }, parser.GetModel().GetDirectories().size());
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code"));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code" / "applications"));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code" / "applications" / "serializer"));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code" / "libraries"));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code" / "libraries" / "serialization"));
    EXPECT_NOT_NULL(parser.GetModel().FindDirectory(rootDirectory / "code" / "libraries" / "serialization" / "test"));

    std::string actualOutput = parser.Serialize();
    auto actualOutputPath = rootDirectory / "parser_out.json";
    auto actualParserOutput = std::ofstream(actualOutputPath, std::ios_base::out);
    actualParserOutput.write(actualOutput.c_str(), static_cast<std::streamsize>(actualOutput.size()));
    actualParserOutput.close();

    std::string expectedOutput;
    auto expectedOutputPath = rootDirectory / "parser.json";
    auto expectedParserOutput = std::ifstream(expectedOutputPath, std::ios_base::in);
    const size_t BufferSize = 4096;
    char buffer[BufferSize];
    while (expectedParserOutput.read(buffer, BufferSize))
    {
        expectedOutput += std::string(buffer, static_cast<std::size_t>(expectedParserOutput.gcount()));
    }
    expectedOutput += std::string(buffer, static_cast<std::size_t>(expectedParserOutput.gcount()));
    expectedParserOutput.close();
    EXPECT_EQ(expectedOutput, actualOutput);
}

} // namespace cmake_parser
