#include "cmake-parser/ProjectModel.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class ProjectModelTest
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

TEST_F(ProjectModelTest, Construct)
{
    std::string projectName = "project";
    ProjectModel project(projectName);

    EXPECT_EQ(projectName, project.Name());
}

} // namespace cmake_parser
