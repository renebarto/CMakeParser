#include "cmake-parser/CMakeCache.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class CMakeCacheTest
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

TEST_F(CMakeCacheTest, Construct)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("CMakeCache:\n", cache.Serialize());
}

TEST_F(CMakeCacheTest, ConstructCopy)
{
    CMakeCache other;
    other.SetVariable("a", "x");
    other.SetVariable("b", "y");
    other.SetVariable("c", "z");

    CMakeCache cache(other);

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("z", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, cache.GetVariables().size());
    EXPECT_EQ("x", cache.GetVariable("a"));
    EXPECT_NOT_NULL(cache.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), cache.FindVariable("a"));
    EXPECT_EQ("y", cache.GetVariable("b"));
    EXPECT_NOT_NULL(cache.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), cache.FindVariable("b"));
    EXPECT_EQ("z", cache.GetVariable("c"));
    EXPECT_NOT_NULL(cache.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), cache.FindVariable("c"));
    EXPECT_EQ("CMakeCache:\nVariable a = x\nVariable b = y\nVariable c = z\n", cache.Serialize());
}

TEST_F(CMakeCacheTest, Assign)
{
    CMakeCache other;
    other.SetVariable("a", "x");
    other.SetVariable("b", "y");
    other.SetVariable("c", "z");

    CMakeCache cache;
    
    cache = other;

    EXPECT_EQ(size_t{ 3 }, other.GetVariables().size());
    EXPECT_EQ("x", other.GetVariable("a"));
    EXPECT_NOT_NULL(other.FindVariable("a"));
    EXPECT_EQ("y", other.GetVariable("b"));
    EXPECT_NOT_NULL(other.FindVariable("b"));
    EXPECT_EQ("z", other.GetVariable("c"));
    EXPECT_NOT_NULL(other.FindVariable("c"));
    EXPECT_EQ(size_t{ 3 }, cache.GetVariables().size());
    EXPECT_EQ("x", cache.GetVariable("a"));
    EXPECT_NOT_NULL(cache.FindVariable("a"));
    EXPECT_NE(other.FindVariable("a"), cache.FindVariable("a"));
    EXPECT_EQ("y", cache.GetVariable("b"));
    EXPECT_NOT_NULL(cache.FindVariable("b"));
    EXPECT_NE(other.FindVariable("b"), cache.FindVariable("b"));
    EXPECT_EQ("z", cache.GetVariable("c"));
    EXPECT_NOT_NULL(cache.FindVariable("c"));
    EXPECT_NE(other.FindVariable("c"), cache.FindVariable("c"));
}

TEST_F(CMakeCacheTest, SetVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    cache.SetVariable("x", "y");

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("y", cache.GetVariable("x"));
    EXPECT_NOT_NULL(cache.FindVariable("x"));
}

TEST_F(CMakeCacheTest, UnsetVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    cache.SetVariable("x", "y");

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("y", cache.GetVariable("x"));
    EXPECT_NOT_NULL(cache.FindVariable("x"));

    cache.UnsetVariable("DUMMY");

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("y", cache.GetVariable("x"));
    EXPECT_NOT_NULL(cache.FindVariable("x"));

    cache.UnsetVariable("x");

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("", cache.GetVariable("x"));
    EXPECT_NULL(cache.FindVariable("x"));
}

TEST_F(CMakeCacheTest, AddVariable)
{
    CMakeCache cache;

    EXPECT_EQ(size_t{ 0 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));

    cache.AddVariable("x", std::make_shared<Variable>("x", "y"));

    EXPECT_EQ(size_t{ 1 }, cache.GetVariables().size());
    EXPECT_EQ("", cache.GetVariable("DUMMY"));
    EXPECT_NULL(cache.FindVariable("DUMMY"));
    EXPECT_EQ("y", cache.GetVariable("x"));
    EXPECT_NOT_NULL(cache.FindVariable("x"));
}

TEST_F(CMakeCacheTest, StreamInsertion)
{
    CMakeCache cache;
    cache.SetVariable("a", "x");
    cache.SetVariable("b", "y");
    cache.SetVariable("c", "z");

    EXPECT_EQ(size_t{ 3 }, cache.GetVariables().size());
    EXPECT_EQ("x", cache.GetVariable("a"));
    EXPECT_NOT_NULL(cache.FindVariable("a"));
    EXPECT_EQ("y", cache.GetVariable("b"));
    EXPECT_NOT_NULL(cache.FindVariable("b"));
    EXPECT_EQ("z", cache.GetVariable("c"));
    EXPECT_NOT_NULL(cache.FindVariable("c"));

    std::ostringstream stream;
    stream << cache;
    EXPECT_EQ("CMakeCache:\nVariable a = x\nVariable b = y\nVariable c = z\n", stream.str());
}

} // namespace cmake_parser
