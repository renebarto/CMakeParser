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

TEST_F(ProjectTest, SerializeJSONEmpty)
{
    std::string projectName = "project";
    Project project(projectName);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": null,\n"
        "    \"description\": null,\n"
        "    \"languages\": null,\n"
        "    \"parent\": null,\n"
        "    \"targets\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(ProjectTest, SerializeJSONNoParentNoTargets)
{
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    Project project(projectName);
    project.SetVersion(version);
    project.SetDescription(description);
    project.SetLanguages(languages);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"parent\": null,\n"
        "    \"targets\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(ProjectTest, SerializeJSONWithParentNoTargets)
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

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"parent\": \"" + parentName + "\",\n"
        "    \"targets\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(ProjectTest, SerializeJSONWithParentAndTargets)
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
    std::string name1{ "target1" };
    std::string name2{ "target2" };
    std::string name3{ "target3" };
    std::string sources1{ "main1.cpp" };
    std::string sources2{ "main2.cpp" };
    std::string sources3{ "" };
    std::string aliasTarget3{ "aliasTarget" };
    auto target1 = std::make_shared<Target>(name1, TargetAttribute::None, sources1);
    auto target2 = std::make_shared<Target>(name2, TargetAttribute::None, sources2);
    auto target3 = std::make_shared<Target>(name3, TargetAttribute::Alias, sources3, aliasTarget3);
    project.AddTarget(target1);
    project.AddTarget(target2);
    project.AddTarget(target3);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"parent\": \"" + parentName + "\",\n"
        "    \"targets\": \n"
        "        [\n"
        "            {\n"
        "                \"name\": \"" + name1 + "\",\n"
        "                \"attributes\": \"None\",\n"
        "                \"sources\": \"" + sources1 + "\"\n"
        "            },\n"
        "            {\n"
        "                \"name\": \"" + name2 + "\",\n"
        "                \"attributes\": \"None\",\n"
        "                \"sources\": \"" + sources2 + "\"\n"
        "            },\n"
        "            {\n"
        "                \"name\": \"" + name3 + "\",\n"
        "                \"attributes\": \"Alias\",\n"
        "                \"sources\": null,\n"
        "                \"aliasTarget\": \"" + aliasTarget3 + "\"\n"
        "            }\n"
        "        ]\n"
        "}", stream.str());
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
