#include "cmake-parser/List.h"

#include "test-platform/GoogleTest.h"

namespace cmake_parser {

class ListTest
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

TEST_F(ListTest, Construct)
{
    List list;

    EXPECT_EQ(size_t{ 0 }, list.Count());
    EXPECT_EQ(std::vector<std::string>{}, list.Items());
    EXPECT_EQ("", list.ToString());
    EXPECT_EQ("List ", list.Serialize());
}

TEST_F(ListTest, ConstructContents)
{
    List list("a;b;c");

    EXPECT_EQ(size_t{ 3 }, list.Count());
    std::vector<std::string> expected{ "a", "b", "c" };
    EXPECT_EQ(expected, list.Items());
    EXPECT_EQ("a;b;c", list.ToString());
    EXPECT_EQ("List a;b;c", list.Serialize());
}

TEST_F(ListTest, Prepend)
{
    List list("a;b;c");

    EXPECT_EQ(size_t{ 3 }, list.Count());
    list.Prepend("d;e;f");
    EXPECT_EQ(size_t{ 6 }, list.Count());
    std::vector<std::string> expected{ "d", "e", "f", "a", "b", "c" };
    EXPECT_EQ(expected, list.Items());
    EXPECT_EQ("d;e;f;a;b;c", list.ToString());
    EXPECT_EQ("List d;e;f;a;b;c", list.Serialize());
}

TEST_F(ListTest, Append)
{
    List list("a;b;c");

    EXPECT_EQ(size_t{ 3 }, list.Count());
    list.Append("d;e;f");
    EXPECT_EQ(size_t{ 6 }, list.Count());
    std::vector<std::string> expected{ "a", "b", "c", "d", "e", "f" };
    EXPECT_EQ(expected, list.Items());
    EXPECT_EQ("a;b;c;d;e;f", list.ToString());
    EXPECT_EQ("List a;b;c;d;e;f", list.Serialize());
}

TEST_F(ListTest, SerializeJSONEmpty)
{
    List list;

    std::ostringstream stream;
    stream << list.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[]", stream.str());
}

TEST_F(ListTest, SerializeJSON)
{
    std::string contents{ "a;b;c" };
    List list(contents);

    std::ostringstream stream;
    stream << list.Serialize(SerializationFormat::JSON, 0);
    EXPECT_EQ(
        "[\n"
        "    \"a\",\n"
        "    \"b\",\n"
        "    \"c\"\n"
        "]", stream.str());
}

TEST_F(ListTest, StreamInsertion)
{
    std::string contents{ "a;b;c" };
    List list(contents);

    std::ostringstream stream;
    stream << list;
    EXPECT_EQ("List " + contents, stream.str());
}

TEST_F(ListTest, StreamInsertionPtr)
{
    std::string contents{ "a;b;c" };
    auto list = std::make_shared<List>(contents);

    std::ostringstream stream;
    stream << list;
    EXPECT_EQ("List " + contents, stream.str());
}

} // namespace cmake_parser
