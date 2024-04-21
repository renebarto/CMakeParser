#include "cmake-parser/DirectoryList.h"

#include "test-platform/GoogleTest.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/Tracing.h"
#include "tracing/TraceWriter.h"

using namespace tracing;

namespace cmake_parser {

class DirectoryListTest
    : public ::testing::Test
{
public:
    CategorySet<TraceCategory> m_savedTraceFilter;
    ConsoleTraceLineWriter m_lineWriter;
    TraceWriter m_traceWriter;

    DirectoryListTest()
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

TEST_F(DirectoryListTest, Construct)
{
    DirectoryList directories;

    EXPECT_EQ(size_t{ 0 }, directories.GetDirectories().size());
    EXPECT_EQ("", directories.GetDirectory("DUMMY"));
    EXPECT_NULL(directories.FindDirectory("DUMMY"));
    EXPECT_EQ("DirectoryList:\n", directories.Serialize());
}

TEST_F(DirectoryListTest, AddDirectoryNullptr)
{
    DirectoryList directories;

    DirectoryPtr directory = nullptr;
    EXPECT_FALSE(directories.AddDirectory(directory));
    EXPECT_EQ(size_t{ 0 }, directories.GetDirectories().size());
}

TEST_F(DirectoryListTest, AddDirectoryExisting)
{
    DirectoryList directories;

    std::filesystem::path path1{ "." };
    auto directory1 = std::make_shared<Directory>(path1, nullptr);
    EXPECT_TRUE(directories.AddDirectory(directory1));

    std::filesystem::path path2{ "." };
    auto directory2 = std::make_shared<Directory>(path2, nullptr);
    EXPECT_FALSE(directories.AddDirectory(directory2));

    EXPECT_EQ(size_t{ 1 }, directories.GetDirectories().size());
    EXPECT_EQ(".", directories.GetDirectory("."));
    EXPECT_EQ(directory1, directories.FindDirectory("."));
}

TEST_F(DirectoryListTest, AddDirectory)
{
    DirectoryList directories;

    std::filesystem::path path{ "." };
    auto directory = std::make_shared<Directory>(path, nullptr);
    directories.AddDirectory(directory);
    EXPECT_EQ(size_t{ 1 }, directories.GetDirectories().size());
    EXPECT_EQ("", directories.GetDirectory("DUMMY"));
    EXPECT_NULL(directories.FindDirectory("DUMMY"));
    EXPECT_EQ(".", directories.GetDirectory("."));
    EXPECT_EQ(directory, directories.FindDirectory("."));
}

TEST_F(DirectoryListTest, AddDirectoryNonExistingParent)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_FALSE(directories.AddDirectory(directory));
    EXPECT_EQ(size_t{ 0 }, directories.GetDirectories().size());
}

TEST_F(DirectoryListTest, AddDirectoryParent)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    EXPECT_EQ(size_t{ 2 }, directories.GetDirectories().size());
    EXPECT_EQ("", directories.GetDirectory("DUMMY"));
    EXPECT_NULL(directories.FindDirectory("DUMMY"));
    EXPECT_EQ(".", directories.GetDirectory("."));
    EXPECT_EQ(parent, directories.FindDirectory("."));
    EXPECT_EQ("subdir", directories.GetDirectory("subdir"));
    EXPECT_EQ(directory, directories.FindDirectory("subdir"));
}

TEST_F(DirectoryListTest, GetSubDirectories)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    EXPECT_EQ(parent, directories.GetRootDirectory());

    auto subDirectories = directories.GetSubDirectories(parent);
    EXPECT_EQ(size_t{ 1 }, subDirectories.size());
    auto it = subDirectories.begin();
    ASSERT_FALSE(it == subDirectories.end());
    EXPECT_EQ(directory, it->second);
    subDirectories = directories.GetSubDirectories(directory);
    EXPECT_EQ(size_t{ 0 }, subDirectories.size());
}

TEST_F(DirectoryListTest, GetRootDirectory)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    EXPECT_EQ(parent, directories.GetRootDirectory());
}

TEST_F(DirectoryListTest, GetParentDirectory)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    EXPECT_EQ(parent, directories.GetParentDirectory(directory));
    EXPECT_EQ(nullptr, directories.GetParentDirectory(parent));
}

TEST_F(DirectoryListTest, Serialize)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    EXPECT_EQ(
        "DirectoryList:\n"
        "Directory path = \"" + parentPath.string() + "\", parent = \"none\"\n"
        "Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"\n", directories.Serialize());
}

TEST_F(DirectoryListTest, StreamInsertion)
{
    DirectoryList directories;

    std::filesystem::path parentPath{ "." };
    auto parent = std::make_shared<Directory>(parentPath, nullptr);
    EXPECT_TRUE(directories.AddDirectory(parent));

    std::filesystem::path path{ "subdir" };
    auto directory = std::make_shared<Directory>(path, parent);
    EXPECT_TRUE(directories.AddDirectory(directory));

    std::ostringstream stream;
    stream << directories;
    EXPECT_EQ(
        "DirectoryList:\n"
        "Directory path = \"" + parentPath.string() + "\", parent = \"none\"\n"
        "Directory path = \"" + path.string() + "\", parent = \"" + parentPath.string() + "\"\n", directories.Serialize());
}

} // namespace cmake_parser
