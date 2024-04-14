#include "test-platform/GoogleTest.h"

#include "parser/Tokenizer.h"
#include "parser/TokenizerRule.h"

namespace parser {

class TokenizerTest
    : public ::testing::Test
{
public:
    void SetUp() override
    {
        SetTokenizerRules({
            { "/(/)(.*)", TokenType{1} },
            });
    }
    void TearDown() override
    {
        SetTokenizerRules({});
    }
};

TEST_F(TokenizerTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("");
    Tokenizer tokenizer(compilationUnit, stream);

    EXPECT_TRUE(tokenizer.GetToken().IsNull());
}

TEST_F(TokenizerTest, GetToken)
{
    std::string compilationUnit("ABC");
    std::string text{ "// Comment" };
    std::istringstream stream(text);
    Tokenizer tokenizer(compilationUnit, stream);

    auto Token = tokenizer.GetToken();
    EXPECT_FALSE(Token.IsNull());
    EXPECT_EQ(TokenType{ 1 }, Token.Type());
    EXPECT_EQ(text, Token.Value());
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), Token.Location());
}

TEST_F(TokenizerTest, GetTokenGarbage)
{
    std::string compilationUnit("ABC");
    std::string text{ "void main() {}" };
    std::istringstream stream(text);
    Tokenizer tokenizer(compilationUnit, stream);

    auto Token = tokenizer.GetToken();
    EXPECT_FALSE(Token.IsNull());
    EXPECT_EQ(TokenType::InvalidToken, Token.Type());
    EXPECT_EQ(text, Token.Value());
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), Token.Location());
}

} // namespace parser
