#include "parser/Lexer.h"

#include "gtest/gtest.h"

using namespace parser;

class LexerTest
    : public ::testing::Test
{
public:
    std::string path;
    std::string code;
    std::istringstream stream;
    LexerTest()
        : path{ "abc.cpp" }
        , code{ "1\n2\r\n3" }
        , stream{ code }
    {}
};

TEST_F(LexerTest, Construct)
{
    Lexer lexer(path, stream);
}

TEST_F(LexerTest, SplitIntoTerms)
{
    Lexer lexer(path, stream);
    lexer.SplitIntoTerms();
    auto& terms = lexer.GetTerms();

    Term term1{ path, SourceLocation{path, 1, 1}, "1" };
    Term term2{ path, SourceLocation{path, 2, 1}, "2" };
    Term term3{ path, SourceLocation{path, 3, 1}, "3" };

    Term term;
    EXPECT_TRUE(terms.NextTerm(term));
    EXPECT_EQ(term1, term);
    EXPECT_TRUE(terms.NextTerm(term));
    EXPECT_EQ(term2, term);
    EXPECT_TRUE(terms.NextTerm(term));
    EXPECT_EQ(term3, term);
}

