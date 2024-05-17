#include "cmake-parser/Project.h"

#include "test-platform/GoogleTest.h"
#include "cmake-parser/Directory.h"

namespace cmake_parser {

class ProjectTest
    : public ::testing::Test
{
public:
    DirectoryPtr directory;

    ProjectTest()
        : directory{ std::make_shared<Directory>("", "") }
    {
    }
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ProjectTest, ConstructDefault)
{
    Project project(directory);

    EXPECT_EQ("", project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = , version = , description = , languages = , homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, Construct)
{
    std::string projectName = "project";
    Project project(directory, projectName);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, ConstructWithParentNull)
{
    std::string projectName = "project";
    Project project(directory, projectName, nullptr);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, ConstructWithParent)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(directory, parentName);
    std::string projectName = "project";
    Project project(directory, projectName, parent);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(parent, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , homepage_url = , parent = " + parentName, project.Serialize());
}

TEST_F(ProjectTest, SetVersion)
{
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    Project project(directory, projectName);
    project.SetVersion(version);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ(version, project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = , languages = , homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetDescription)
{
    std::string projectName = "project";
    std::string description = "blablabla";
    Project project(directory, projectName);
    project.SetDescription(description);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ(description, project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = " + description + ", languages = , homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetLanguages)
{
    std::string projectName = "project";
    std::string languages = "C CXX";
    Project project(directory, projectName);
    project.SetLanguages(languages);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ(languages, project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = " + languages + ", homepage_url = , parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetHomePageURL)
{
    std::string projectName = "project";
    std::string url = "http://myproject.com";
    Project project(directory, projectName);
    project.SetHomePageURL(url);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ(url, project.HomePageURL());
    EXPECT_EQ(nullptr, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , homepage_url = " + url + ", parent = none", project.Serialize());
}

TEST_F(ProjectTest, SetParent)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(directory, parentName);

    std::string projectName = "project";
    Project project(directory, projectName);
    project.SetParent(parent);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ("", project.Version());
    EXPECT_EQ("", project.Description());
    EXPECT_EQ("", project.Languages());
    EXPECT_EQ("", project.HomePageURL());
    EXPECT_EQ(parent, project.Parent());
    EXPECT_EQ("Project name = " + projectName + ", version = , description = , languages = , homepage_url = , parent = " + parentName, project.Serialize());
}

TEST_F(ProjectTest, SerializeJSONEmpty)
{
    std::string projectName = "project";
    Project project(directory, projectName);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": null,\n"
        "    \"description\": null,\n"
        "    \"languages\": null,\n"
        "    \"homepage_url\": null,\n"
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
    std::string url = "http://myproject.com";
    Project project(directory, projectName);
    project.SetVersion(version);
    project.SetDescription(description);
    project.SetLanguages(languages);
    project.SetHomePageURL(url);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"homepage_url\": \"" + url + "\",\n"
        "    \"parent\": null,\n"
        "    \"targets\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(ProjectTest, SerializeJSONWithParentNoTargets)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(directory, parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    std::string url = "http://myproject.com";
    Project project(directory, projectName, parent);
    project.SetVersion(version);
    project.SetDescription(description);
    project.SetLanguages(languages);
    project.SetHomePageURL(url);

    std::ostringstream stream;
    stream << project.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"homepage_url\": \"" + url + "\",\n"
        "    \"parent\": \"" + parentName + "\",\n"
        "    \"targets\": \n"
        "        []\n"
        "}", stream.str());
}

TEST_F(ProjectTest, SerializeJSONWithParentAndTargets)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(directory, parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    std::string url = "http://myproject.com";
    ProjectPtr project = std::make_shared<Project>(directory, projectName, parent);
    project->SetVersion(version);
    project->SetDescription(description);
    project->SetLanguages(languages);
    project->SetHomePageURL(url);
    std::string name1{ "target1" };
    std::string name2{ "target2" };
    std::string name3{ "target3" };
    std::string sources1{ "main1.cpp" };
    std::string sources2{ "main2.cpp" };
    std::string sources3{ "" };
    std::string aliasTarget3{ "aliasTarget" };
    auto target1 = std::make_shared<Target>(project, name1, TargetType::Executable, TargetAttribute::None, sources1);
    auto target2 = std::make_shared<Target>(project, name2, TargetType::StaticLibrary, TargetAttribute::None, sources2);
    auto target3 = std::make_shared<Target>(project, name3, TargetType::Object, TargetAttribute::Alias, sources3, aliasTarget3);
    project->AddTarget(target1);
    project->AddTarget(target2);
    project->AddTarget(target3);

    std::ostringstream stream;
    stream << project->Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "{\n"
        "    \"name\": \"" + projectName + "\",\n"
        "    \"version\": \"" + version + "\",\n"
        "    \"description\": \"" + description + "\",\n"
        "    \"languages\": \"" + languages + "\",\n"
        "    \"homepage_url\": \"" + url + "\",\n"
        "    \"parent\": \"" + parentName + "\",\n"
        "    \"targets\": \n"
        "        [\n"
        "            {\n"
        "                \"name\": \"" + name1 + "\",\n"
        "                \"type\": \"Executable\",\n"
        "                \"attributes\": \"None\",\n"
        "                \"sources\": \"" + sources1 + "\"\n"
        "            },\n"
        "            {\n"
        "                \"name\": \"" + name2 + "\",\n"
        "                \"type\": \"StaticLibrary\",\n"
        "                \"attributes\": \"None\",\n"
        "                \"sources\": \"" + sources2 + "\"\n"
        "            },\n"
        "            {\n"
        "                \"name\": \"" + name3 + "\",\n"
        "                \"type\": \"Object\",\n"
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
    auto parent = std::make_shared<Project>(directory, parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    std::string url = "http://myproject.com";
    Project project(directory, projectName, parent);
    project.SetVersion(version);
    project.SetDescription(description);
    project.SetLanguages(languages);
    project.SetHomePageURL(url);

    EXPECT_EQ(projectName, project.Name());
    EXPECT_EQ(version, project.Version());
    EXPECT_EQ(description, project.Description());
    EXPECT_EQ(languages, project.Languages());
    std::ostringstream stream;
    stream << project;
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = " + description + ", languages = " + languages + ", homepage_url = " + url + ", parent = " + parentName, stream.str());
}

TEST_F(ProjectTest, StreamInsertionPtr)
{
    std::string parentName = "parent";
    auto parent = std::make_shared<Project>(directory, parentName);
    std::string projectName = "project";
    std::string version = "1.2.3.4";
    std::string description = "blablabla";
    std::string languages = "C CXX";
    std::string url = "http://myproject.com";
    auto project = std::make_shared<Project>(directory, projectName, parent);
    project->SetVersion(version);
    project->SetDescription(description);
    project->SetLanguages(languages);
    project->SetHomePageURL(url);

    EXPECT_EQ(projectName, project->Name());
    EXPECT_EQ(version, project->Version());
    EXPECT_EQ(description, project->Description());
    EXPECT_EQ(languages, project->Languages());
    std::ostringstream stream;
    stream << project;
    EXPECT_EQ("Project name = " + projectName + ", version = " + version + ", description = " + description + ", languages = " + languages + ", homepage_url = " + url + ", parent = " + parentName, stream.str());
}

} // namespace cmake_parser
