#include "cmake-parser/CMakeParser.h"

#include "test-platform/GoogleTest.h"
#include <filesystem>
#include <fstream>
#include "utility/CommandLine.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {

class CMakeParserTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    CMakeParserTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
        Tracing::SetDefaultTraceFilter(TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information/* | TraceCategory::Data | TraceCategory::Debug*/);
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

TEST_F(CMakeParserTest, Construct)
{
    std::filesystem::path rootDirectory{ TEST_DATA_DIR };
    std::string buildDir{ "cmake-x64-Debug" };
    std::ifstream stream(rootDirectory / CMakeScriptFileName);
    CMakeParser parser(rootDirectory, buildDir, stream);

    EXPECT_TRUE(parser.Parse());
    const std::filesystem::path serializationTestSourceDir{ rootDirectory / "code" / "libraries" / "serialization" / "test" };
    EXPECT_EQ(size_t{ 0 }, parser.GetModel().GetEnvironmentVariables().size());
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
    EXPECT_EQ(size_t{ 72 }, parser.GetModel().GetVariables().size());
    EXPECT_EQ(ARGC, parser.GetModel().GetVariable("ARGC"));
    EXPECT_EQ(ARGN, parser.GetModel().GetVariable("ARGN"));
    EXPECT_EQ(ARGV, parser.GetModel().GetVariable("ARGV"));
    EXPECT_EQ(CMAKE_BUILD_TYPE, parser.GetModel().GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ(CMAKE_CXX_COMPILER, parser.GetModel().GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ(CMAKE_C_COMPILER, parser.GetModel().GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ(CMAKE_FILES_DIRECTORY, parser.GetModel().GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ(CMAKE_GENERATOR_INSTANCE, parser.GetModel().GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ(CMAKE_GENERATOR_PLATFORM, parser.GetModel().GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ(CMAKE_GENERATOR_TOOLSET, parser.GetModel().GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ(CMAKE_HOME_DIRECTORY, parser.GetModel().GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ(CMAKE_HOST_SYSTEM_NAME, parser.GetModel().GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ(CMAKE_HOST_WIN32, parser.GetModel().GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ(ISWIN32, parser.GetModel().GetVariable("WIN32"));
    EXPECT_EQ(CMAKE_SOURCE_DIR, parser.GetModel().GetVariable(VarMainSourceDirectory));
    EXPECT_EQ(CMAKE_BINARY_DIR, parser.GetModel().GetVariable(VarMainBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_SOURCE_DIR, parser.GetModel().GetVariable(VarCurrentSourceDirectory));
    EXPECT_EQ(CMAKE_CURRENT_BINARY_DIR, parser.GetModel().GetVariable(VarCurrentBinaryDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_DIR, parser.GetModel().GetVariable(VarCurrentScriptDirectory));
    EXPECT_EQ(CMAKE_CURRENT_LIST_FILE, parser.GetModel().GetVariable(VarCurrentScriptPath));
    EXPECT_EQ(CMAKE_PARENT_LIST_FILE, parser.GetModel().GetVariable(VarParentScriptPath));
    EXPECT_EQ(CMAKE_COMMAND, parser.GetModel().GetVariable(VarCMakeExePath));
    EXPECT_EQ(CMAKE_CPACK_COMMAND, parser.GetModel().GetVariable(VarCPackExePath));
    EXPECT_EQ(CMAKE_CTEST_COMMAND, parser.GetModel().GetVariable(VarCTestExePath));
    EXPECT_EQ(CMAKE_MAJOR_VERSION, parser.GetModel().GetVariable(VarCMakeVersionMajor));
    EXPECT_EQ(CMAKE_MINOR_VERSION, parser.GetModel().GetVariable(VarCMakeVersionMinor));
    EXPECT_EQ(CMAKE_PATCH_VERSION, parser.GetModel().GetVariable(VarCMakeVersionPatch));
    EXPECT_EQ(CMAKE_ROOT, parser.GetModel().GetVariable(VarCMakeRootPath));
    EXPECT_EQ(CMAKE_TWEAK_VERSION, parser.GetModel().GetVariable(VarCMakeVersionTweak));
    EXPECT_EQ(CMAKE_VERSION, parser.GetModel().GetVariable(VarCMakeVersion));
    EXPECT_EQ(CMAKE_GENERATOR, parser.GetModel().GetVariable(VarCMakeGenerator));
    EXPECT_EQ(CMAKE_MAKE_PROGRAM, parser.GetModel().GetVariable(VarMakeProgramPath));

    EXPECT_EQ("3.5.1", parser.GetModel().GetVariable("CMAKE_MINIMUM_REQUIRED_VERSION"));

    const std::string platformName{ "windows" };
    EXPECT_EQ("1.2.3.4", parser.GetModel().GetVariable("MSI_NUMBER"));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable("PLATFORM_WINDOWS"));
    EXPECT_EQ(platformName, parser.GetModel().GetVariable("PLATFORM_NAME"));
    EXPECT_EQ("ON", parser.GetModel().GetVariable("CMAKE_EXPORT_COMPILE_COMMANDS"));
    EXPECT_EQ("OFF", parser.GetModel().GetVariable("CMAKE_COLOR_MAKEFILE"));
    EXPECT_EQ("ON", parser.GetModel().GetVariable("CMAKE_BUILD_WITH_INSTALL_RPATH"));
    EXPECT_EQ("OFF", parser.GetModel().GetVariable("CMAKE_VERBOSE_MAKEFILE"));
    EXPECT_EQ(CMAKE_SOURCE_DIR / "output" / platformName, parser.GetModel().GetVariable("OUTPUT_BASE_DIR"));
    EXPECT_EQ(CMAKE_SOURCE_DIR / "testdata", parser.GetModel().GetVariable("TEST_DATA_DIR"));
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
    EXPECT_EQ("_UNICODE;UNICODE;_CRT_SECURE_NO_WARNINGS;_SCL_SECURE_NO_WARNINGS;_WINSOCK_DEPRECATED_NO_WARNINGS;WINVER=0x0A00;_WIN32_WINNT=0x0A00;PLATFORM_WINDOWS;TEST_DATA_DIR=\"D:/Projects/CPPParser/testdata/testdata\";_AMD64_;_X86_", 
        parser.GetModel().GetVariable("DEFINES"));
    EXPECT_EQ("_DEBUG", parser.GetModel().GetVariable("DEFINES_DEBUG"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_RELEASE"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_MINSIZEREL"));
    EXPECT_EQ("NDEBUG", parser.GetModel().GetVariable("DEFINES_RELWITHDEBINFO"));
    EXPECT_EQ("/ignore:4217", parser.GetModel().GetVariable("LINK_FLAGS"));
    EXPECT_EQ("TRUE", parser.GetModel().GetVariable("CMAKE_USE_WIN32_THREADS_INIT"));
    EXPECT_EQ("NEW", parser.GetModel().GetVariable("CMAKE_POLICY_DEFAULT_CMP0048"));

    EXPECT_EQ(size_t{ 4 }, parser.GetModel().GetProjects().size());
    EXPECT_EQ(size_t{ 7 }, parser.GetModel().GetDirectories().size());
    std::string directoryPath{ TEST_DATA_DIR };
    auto directory = parser.GetModel().FindDirectory(directoryPath);
    ASSERT_NOT_NULL(directory);
    EXPECT_EQ(directoryPath, directory->SourcePath());
    EXPECT_NULL(directory->Parent());
}

} // namespace cmake_parser
