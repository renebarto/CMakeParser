#include "test-platform/GoogleTest.h"

#include "parser/ParserExecutor.h"
#include "parser/Tokenizer.h"
#include "parser/TokenizerRule.h"
#include "ParserCallbackMock.h"

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::SaveArg;
using ::testing::NaggyMock;

namespace parser {

enum TokenTypes
{
    None,
    Whitespace,
    NewLine,
    SingleLineComment,
    BlockComment,
    Identifier,
    ParenthesisOpen,
    ParenthesisClose,
    CurlyBraceOpen,
    CurlyBraceClose,
};

class ParserExecutorTest
    : public ::testing::Test
{
public:
    void TearDown() override
    {
        SetupTokenDefinitions<TokenTypes>({});
        SetTokenizerRules<TokenTypes>({});
    }
};

TEST_F(ParserExecutorTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("");
    ParserCallbackMock<TokenTypes> callback;

    EXPECT_CALL(callback, OnToken(_, _)).Times(0);
    EXPECT_CALL(callback, OnParseError(_)).Times(0);
    EXPECT_CALL(callback, OnNoMoreToken(_)).WillOnce(Return(true));

    Tokenizer<TokenTypes> tokenizer(compilationUnit, stream);
    ParserExecutor<TokenTypes> parser(callback, { TokenTypes::Whitespace });
    EXPECT_TRUE(parser.Parse(tokenizer));
}

TEST_F(ParserExecutorTest, ParseSimpleComment)
{
    std::string compilationUnit("ABC");
    std::string value{ "// Comment" };
    std::istringstream stream(value);
    ParserCallbackMock<TokenTypes> callback;
    Token<TokenTypes> token;
    SetupTokenDefinitions<TokenTypes>({
        { SingleLineComment, "SingleLineComment" },
        });
    SetTokenizerRules<TokenTypes>({
        { "//.*", SingleLineComment, true },
        });

    EXPECT_CALL(callback, OnToken(_, _)).WillOnce(DoAll(SaveArg<0>(&token), SetArgReferee<1>(false), Return(true)));
    EXPECT_CALL(callback, OnParseError(_)).Times(0);
    EXPECT_CALL(callback, OnNoMoreToken(_)).WillOnce(Return(true));

    Tokenizer<TokenTypes> tokenizer(compilationUnit, stream);
    ParserExecutor<TokenTypes> parser(callback, { TokenTypes::Whitespace });
    EXPECT_TRUE(parser.Parse(tokenizer));
    EXPECT_EQ(TokenType<TokenTypes>{ SingleLineComment }, token.Type());
    EXPECT_EQ(value, token.Value());
}

TEST_F(ParserExecutorTest, ParseInvalid)
{
    std::string compilationUnit("ABC");
    std::string text{ "void main(void {})"};
    std::istringstream stream(text);
    ParserCallbackMock<TokenTypes>callback;
    Token<TokenTypes> errorToken;
    SetupTokenDefinitions<TokenTypes>({
        { SingleLineComment, "SingleLineComment" },
        });
    SetTokenizerRules<TokenTypes>({
        { "//.*", SingleLineComment, true },
        });

    EXPECT_CALL(callback, OnToken(_, _)).Times(0);
    EXPECT_CALL(callback, OnParseError(_)).WillOnce(DoAll(SaveArg<0>(&errorToken)));
    EXPECT_CALL(callback, OnNoMoreToken(_)).Times(0);

    Tokenizer<TokenTypes> tokenizer(compilationUnit, stream);
    ParserExecutor<TokenTypes> parser(callback, { TokenTypes::Whitespace });
    EXPECT_FALSE(parser.Parse(tokenizer));
    EXPECT_EQ(text, errorToken.Value());
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 1), errorToken.BeginLocation());
    EXPECT_EQ(SourceLocation(compilationUnit, 1, 19), errorToken.EndLocation());
}

TEST_F(ParserExecutorTest, ParseComplex)
{
    std::string compilationUnit("ABC");
    std::string text{ "//Comment\nvoid main(void) {/* Do something*/}" };
    std::istringstream stream(text);
    ParserCallbackMock<TokenTypes> callback;
    Token<TokenTypes> token1;
    Token<TokenTypes> token2;
    Token<TokenTypes> token3;
    Token<TokenTypes> token4;
    Token<TokenTypes> token5;
    Token<TokenTypes> token6;
    Token<TokenTypes> token7;
    Token<TokenTypes> token8;
    Token<TokenTypes> token9;
    Token<TokenTypes> token10;
    Token<TokenTypes> token11;
    Token<TokenTypes> token12;
    SetupTokenDefinitions<TokenTypes>({
        { None, "None" },
        { Whitespace, "Whitespace" },
        { NewLine, "NewLine" },
        { SingleLineComment, "SingleLineComment" },
        { Identifier, "Identifier" },
        { ParenthesisOpen, "ParenthesisOpen" },
        { ParenthesisClose, "ParenthesisClose" },
        { CurlyBraceOpen, "CurlyBraceOpen" },
        { CurlyBraceClose, "CurlyBraceClose" },
        });
    SetTokenizerRules<TokenTypes>({
        { "[ \t]+", Whitespace },
        { "(\r)?\n", NewLine },
        { "//[^\r\n]*", SingleLineComment },
        { "/\\*[\\s\\S]*?\\*/", {BlockComment} },
        { "[_a-zA-Z][_a-zA-Z0-9]*", Identifier },
        { "\\(", ParenthesisOpen },
        { "\\)", ParenthesisClose },
        { "\\{", CurlyBraceOpen},
        { "\\}", CurlyBraceClose },
        });

    EXPECT_CALL(callback, OnToken(_, _))
        .WillOnce(DoAll(SaveArg<0>(&token1), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token2), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token3), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token4), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token5), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token6), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token7), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token8), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token9), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token10), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token11), SetArgReferee<1>(false), Return(true)))
        .WillOnce(DoAll(SaveArg<0>(&token12), SetArgReferee<1>(false), Return(true)));
    EXPECT_CALL(callback, OnParseError(_)).Times(0);
    EXPECT_CALL(callback, OnNoMoreToken(_)).WillOnce(Return(true));

    Tokenizer<TokenTypes> tokenizer(compilationUnit, stream);
    ParserExecutor<TokenTypes> parser(callback, {});
    EXPECT_TRUE(parser.Parse(tokenizer));

    EXPECT_EQ(TokenType<TokenTypes>{ SingleLineComment }, token1.Type());
    EXPECT_EQ("//Comment", token1.Value());
    
    EXPECT_EQ(TokenType<TokenTypes>{ NewLine }, token2.Type());
    EXPECT_EQ("\n", token2.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ Identifier }, token3.Type());
    EXPECT_EQ("void", token3.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ Whitespace }, token4.Type());
    EXPECT_EQ(" ", token4.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ Identifier }, token5.Type());
    EXPECT_EQ("main", token5.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ ParenthesisOpen }, token6.Type());
    EXPECT_EQ("(", token6.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ Identifier }, token7.Type());
    EXPECT_EQ("void", token7.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ ParenthesisClose }, token8.Type());
    EXPECT_EQ(")", token8.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ Whitespace }, token9.Type());
    EXPECT_EQ(" ", token9.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ CurlyBraceOpen }, token10.Type());
    EXPECT_EQ("{", token10.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ BlockComment }, token11.Type());
    EXPECT_EQ("/* Do something*/", token11.Value());

    EXPECT_EQ(TokenType<TokenTypes>{ CurlyBraceClose }, token12.Type());
    EXPECT_EQ("}", token12.Value());
}

} // namespace parser
