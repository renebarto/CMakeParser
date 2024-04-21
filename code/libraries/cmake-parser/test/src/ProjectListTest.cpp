#include "cmake-parser/ProjectList.h"

#include "test-platform/GoogleTest.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {

class ProjectListTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    ProjectListTest()
        : m_savedTraceFilter{}
        , m_lineWriter{}
        , m_traceWriter(m_lineWriter)
    {
    }
    void SetUp() override
    {
        Tracing::SetTraceWriter(&m_traceWriter);
        m_savedTraceFilter = Tracing::GetDefaultTraceFilter();
    }
    void TearDown() override
    {
        Tracing::SetTraceWriter(nullptr);
        Tracing::SetDefaultTraceFilter(m_savedTraceFilter);
    }
};

TEST_F(ProjectListTest, Construct)
{
    ProjectList projects;

    EXPECT_EQ(size_t{ 0 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));
    EXPECT_EQ("ProjectList:\n", projects.Serialize());
}

TEST_F(ProjectListTest, AddProjectNullptr)
{
    ProjectList projects;

    EXPECT_EQ(size_t{ 0 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));

    ProjectPtr project = nullptr;
    EXPECT_FALSE(projects.AddProject(project));
}

TEST_F(ProjectListTest, AddProjectExisting)
{
    ProjectList projects;

    EXPECT_EQ(size_t{ 0 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));

    std::string projectName1 = "project";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_FALSE(projects.AddProject(project2));
}

TEST_F(ProjectListTest, AddProjectNonExistingParent)
{
    ProjectList projects;

    EXPECT_EQ(size_t{ 0 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_FALSE(projects.AddProject(project2));

    EXPECT_TRUE(projects.AddProject(project1));

    EXPECT_TRUE(projects.AddProject(project2));
}

TEST_F(ProjectListTest, AddProject)
{
    ProjectList projects;

    EXPECT_EQ(size_t{ 0 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    EXPECT_EQ(size_t{ 2 }, projects.GetProjects().size());
    EXPECT_NULL(projects.GetProject("DUMMY"));
    EXPECT_EQ(project1, projects.GetProject(projectName1));
    EXPECT_EQ(project2, projects.GetProject(projectName2));
}

TEST_F(ProjectListTest, GetSubProjects)
{
    ProjectList projects;

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    auto subProjects = projects.GetSubProjects(project1);
    EXPECT_EQ(size_t{ 1 }, subProjects.size());
    auto it = subProjects.begin();
    ASSERT_FALSE(it == subProjects.end());
    EXPECT_EQ(project2, it->second);
    subProjects = projects.GetSubProjects(project2);
    EXPECT_EQ(size_t{ 0 }, subProjects.size());
}

TEST_F(ProjectListTest, GetMainProject)
{
    ProjectList projects;

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    EXPECT_EQ(project1, projects.GetMainProject());
}

TEST_F(ProjectListTest, GetParentProject)
{
    ProjectList projects;

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    EXPECT_EQ(nullptr, projects.GetParentProject(project1));
    EXPECT_EQ(project1, projects.GetParentProject(project2));
}

TEST_F(ProjectListTest, Serialize)
{
    ProjectList projects;

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    EXPECT_EQ(
        "ProjectList:\n"
        "Project name = project1, version = 1.2.3.4, description = blablabla, languages = C CXX, parent = none\n"
        "Project name = project2, version = 1.2.3.5, description = blabla, languages = CXX ASM, parent = project1\n", projects.Serialize());
}

TEST_F(ProjectListTest, StreamInsertion)
{
    ProjectList projects;

    std::string projectName1 = "project1";
    std::string version1 = "1.2.3.4";
    std::string description1 = "blablabla";
    std::string languages1 = "C CXX";
    auto project1 = std::make_shared<Project>(projectName1);
    project1->SetVersion(version1);
    project1->SetDescription(description1);
    project1->SetLanguages(languages1);

    EXPECT_TRUE(projects.AddProject(project1));

    std::string projectName2 = "project2";
    std::string version2 = "1.2.3.5";
    std::string description2 = "blabla";
    std::string languages2 = "CXX ASM";
    auto project2 = std::make_shared<Project>(projectName2, project1);
    project2->SetVersion(version2);
    project2->SetDescription(description2);
    project2->SetLanguages(languages2);

    EXPECT_TRUE(projects.AddProject(project2));

    std::ostringstream stream;
    stream << projects;
    EXPECT_EQ(
        "ProjectList:\n"
        "Project name = project1, version = 1.2.3.4, description = blablabla, languages = C CXX, parent = none\n"
        "Project name = project2, version = 1.2.3.5, description = blabla, languages = CXX ASM, parent = project1\n", stream.str());
}

} // namespace cmake_parser
