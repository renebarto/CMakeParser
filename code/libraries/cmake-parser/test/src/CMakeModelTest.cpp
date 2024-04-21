#include "cmake-parser/CMakeModel.h"

#include "test-platform/GoogleTest.h"
#include "utility/CommandLine.h"

namespace cmake_parser {

class CMakeModelTest
    : public ::testing::Test
{
public:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

static std::string FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
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

static std::string FindNinja()
{
    std::string stdoutText;
    std::string stderrText;
    if (utility::SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

const std::string ARGC{ "0" };

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

    model.SetupSourceRoot(TEST_DATA_DIR);

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 18 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ("", model.GetVariable("ARGN"));
    EXPECT_EQ("", model.GetVariable("ARGV"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_BINARY_DIR"));
    EXPECT_EQ("Debug", model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_CURRENT_BINARY_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_SOURCE_DIR"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ("/CMakeFiles", model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ("", model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ("Windows", model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ("1", model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_SOURCE_DIR"));
    EXPECT_EQ("1", model.GetVariable("WIN32"));
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(TEST_DATA_DIR));
}

TEST_F(CMakeModelTest, SetupRootCMakeFile)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR);
    model.SetupRootCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 21 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ("", model.GetVariable("ARGN"));
    EXPECT_EQ("", model.GetVariable("ARGV"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_BINARY_DIR"));
    EXPECT_EQ("Debug", model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_CURRENT_BINARY_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_LIST_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_CURRENT_LIST_FILE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_SOURCE_DIR"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ("/CMakeFiles", model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ("", model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ("Windows", model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ("1", model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_PARENT_LIST_FILE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_SOURCE_DIR"));
    EXPECT_EQ("1", model.GetVariable("WIN32"));
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(TEST_DATA_DIR));
}

TEST_F(CMakeModelTest, SetupCMakePath)
{
    CMakeModel model;
        
    model.SetupSourceRoot(TEST_DATA_DIR);
    model.SetupRootCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 30 }, model.GetVariables().size());
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ("", model.GetVariable("ARGN"));
    EXPECT_EQ("", model.GetVariable("ARGV"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_BINARY_DIR"));
    EXPECT_EQ("Debug", model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ("C:/Program Files/CMake/bin/cmake.exe", model.GetVariable("CMAKE_COMMAND"));
    EXPECT_EQ("C:/Program Files/CMake/bin/cpack.exe", model.GetVariable("CMAKE_CPACK_COMMAND"));
    EXPECT_EQ("C:/Program Files/CMake/bin/ctest.exe", model.GetVariable("CMAKE_CTEST_COMMAND"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_CURRENT_BINARY_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_LIST_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_CURRENT_LIST_FILE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_SOURCE_DIR"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ("/CMakeFiles", model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ("", model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ("Windows", model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ("1", model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ("3", model.GetVariable("CMAKE_MAJOR_VERSION"));
    EXPECT_EQ("27", model.GetVariable("CMAKE_MINOR_VERSION"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_PARENT_LIST_FILE"));
    EXPECT_EQ("8", model.GetVariable("CMAKE_PATCH_VERSION"));
    EXPECT_EQ("C:/Program Files/CMake/share/cmake-3.27", model.GetVariable("CMAKE_ROOT"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_SOURCE_DIR"));
    EXPECT_EQ("0", model.GetVariable("CMAKE_TWEAK_VERSION"));
    EXPECT_EQ("3.27.8", model.GetVariable("CMAKE_VERSION"));
    EXPECT_EQ("1", model.GetVariable("WIN32"));
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(TEST_DATA_DIR));
}

TEST_F(CMakeModelTest, SetupNinjaPath)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR);
    model.SetupRootCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_TRUE(model.IsSourceRootSet());
    EXPECT_EQ(size_t{ 32 }, model.GetVariables().size());
    EXPECT_EQ(ARGC, model.GetVariable("ARGC"));
    EXPECT_EQ("", model.GetVariable("ARGN"));
    EXPECT_EQ("", model.GetVariable("ARGV"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_BINARY_DIR"));
    EXPECT_EQ("Debug", model.GetVariable("CMAKE_BUILD_TYPE"));
    EXPECT_EQ("C:/Program Files/CMake/bin/cmake.exe", model.GetVariable("CMAKE_COMMAND"));
    EXPECT_EQ("C:/Program Files/CMake/bin/cpack.exe", model.GetVariable("CMAKE_CPACK_COMMAND"));
    EXPECT_EQ("C:/Program Files/CMake/bin/ctest.exe", model.GetVariable("CMAKE_CTEST_COMMAND"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/cmake-x64-Debug", model.GetVariable("CMAKE_CURRENT_BINARY_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_LIST_DIR"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_CURRENT_LIST_FILE"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_CURRENT_SOURCE_DIR"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_CXX_COMPILER"));
    EXPECT_EQ("C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe", model.GetVariable("CMAKE_C_COMPILER"));
    EXPECT_EQ("/CMakeFiles", model.GetVariable("CMAKE_FILES_DIRECTORY"));
    EXPECT_EQ("Ninja", model.GetVariable("CMAKE_GENERATOR"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_INSTANCE"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_PLATFORM"));
    EXPECT_EQ("", model.GetVariable("CMAKE_GENERATOR_TOOLSET"));
    EXPECT_EQ("", model.GetVariable("CMAKE_HOME_DIRECTORY"));
    EXPECT_EQ("Windows", model.GetVariable("CMAKE_HOST_SYSTEM_NAME"));
    EXPECT_EQ("1", model.GetVariable("CMAKE_HOST_WIN32"));
    EXPECT_EQ("3", model.GetVariable("CMAKE_MAJOR_VERSION"));
    EXPECT_EQ("C:\\Program Files\\Ninja\\ninja.exe", model.GetVariable("CMAKE_MAKE_PROGRAM"));
    EXPECT_EQ("27", model.GetVariable("CMAKE_MINOR_VERSION"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata/CMakeLists.txt", model.GetVariable("CMAKE_PARENT_LIST_FILE"));
    EXPECT_EQ("8", model.GetVariable("CMAKE_PATCH_VERSION"));
    EXPECT_EQ("C:/Program Files/CMake/share/cmake-3.27", model.GetVariable("CMAKE_ROOT"));
    EXPECT_EQ("D:/Projects/CPPParser/testdata", model.GetVariable("CMAKE_SOURCE_DIR"));
    EXPECT_EQ("0", model.GetVariable("CMAKE_TWEAK_VERSION"));
    EXPECT_EQ("3.27.8", model.GetVariable("CMAKE_VERSION"));
    EXPECT_EQ("1", model.GetVariable("WIN32"));
    EXPECT_EQ(size_t{ 0 }, model.GetProjects().size());
    EXPECT_EQ(size_t{ 1 }, model.GetDirectories().size());
    EXPECT_NOT_NULL(model.FindDirectory(TEST_DATA_DIR));
}

TEST_F(CMakeModelTest, GetVariables)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR);
    model.SetupRootCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_TRUE(model.IsSourceRootSet());
    auto variables = model.GetVariables();
    ASSERT_TRUE(variables.find("CMAKE_VERSION") != variables.end());
    EXPECT_EQ("3.27.8", variables.find("CMAKE_VERSION")->second->Value());
}

TEST_F(CMakeModelTest, GetVariable)
{
    CMakeModel model;

    model.SetupSourceRoot(TEST_DATA_DIR);
    model.SetupRootCMakeFile(std::string(TEST_DATA_DIR) + "/CMakeLists.txt");
    model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    model.SetupNinjaPath(FindNinja());

    EXPECT_TRUE(model.IsSourceRootSet());
    auto variables = model.GetVariables();
    ASSERT_TRUE(variables.find("CMAKE_VERSION") != variables.end());
    EXPECT_EQ("3.27.8", variables.find("CMAKE_VERSION")->second->Value());
}

TEST_F(CMakeModelTest, AddProject)
{
    CMakeModel model;

    auto projectName = "project1";
    auto project = std::make_shared<Project>(projectName);
    EXPECT_NULL(model.GetProject(projectName));
    EXPECT_TRUE(model.AddProject(project));
    EXPECT_NOT_NULL(model.GetProject(projectName));
    EXPECT_FALSE(model.AddProject(project));
    EXPECT_NOT_NULL(model.GetProject(projectName));
}

} // namespace cmake_parser
