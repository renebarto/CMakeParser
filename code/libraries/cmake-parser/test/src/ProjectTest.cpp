#include "cmake-parser/Project.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class ProjectTest
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

TEST_F(ProjectTest, ConstructDefault)
{
    Project project;

    EXPECT_EQ("", project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = , version = , description = , languages = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, Construct)
{
    std::string projectName = "project";
    Project project(projectName);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, ConstructWithParentNull)
{
    std::string projectName = "project";
    Project project(projectName, nullptr);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, ConstructWithParent)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(parentName);
    std::string projectName = "project";
    Project project(projectName, parent);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(parent, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , parent = " + parentName, project.Serialize());
}

TEST_F(ProjectTest, SetVersion)
{
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    Project project(projectName);
    project.SetVersion(version);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ(version, project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = , languages = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetDescription)
{
    std::string projectName = "project";
    std::string description = "blablabla";
    Project project(projectName);
    project.SetDescription(description);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ(description, project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = " + description + ", languages = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetLanguages)
{
    std::string projectName = "project";
    std::string languages = "C CXX";
    Project project(projectName);
    project.SetLanguages(languages);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ(languages, project.Languages());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = " + languages + ", parent = none", project.Serialize());
}

TEST_F(ProjectTest, StreamInsertion)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    Project project(projectName, parent);
    project.SetVersion(version);
    project.SetDescription(description);
    project.SetLanguages(languages);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ(version, project.Version());
    EXPECT_EQ(description, project.Description());
    EXPECT_EQ(languages, project.Languages());
    std::ostringstream stream;
    stream << project;
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = " + description + ", languages = " + languages + ", parent = " + parentName, stream.str());
}

TEST_F(ProjectTest, StreamInsertionPtr)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    auto project = std::make_shared<Project>(projectName, parent);
    project->SetVersion(version);
    project->SetDescription(description);
    project->SetLanguages(languages);

    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ(version, project->Version());
    EXPECT_EQ(description, project->Description());
    EXPECT_EQ(languages, project->Languages());
    std::ostringstream stream;
    stream << project;
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = " + description + ", languages = " + languages + ", parent = " + parentName, stream.str());
}

} // namespace cmake_parser
