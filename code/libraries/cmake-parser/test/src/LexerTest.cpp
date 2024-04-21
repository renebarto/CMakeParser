#include "cmake-parser/Lexer.h"

#include "test-platform/GoogleTest.h"

#include <filesystem>
#include <fstream>

using namespace parser;

namespace cmake_parser {

class LexerTest
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

TEST_F(LexerTest, EmptyStream)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token<Terminal>{}, lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, Whitespace)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(" \t");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::Whitespace), " \t", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 3)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, NewLine)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("\n\r\n");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 2, 1)), lexer.GetToken());
    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\r\n", SourceLocation("ABC", 2, 1), SourceLocation("ABC", 3, 1)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, SimpleTerminals)
{
    std::string compilationUnit("ABC");
    std::istringstream stream("$(){}");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::Dollar),              "$", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 2)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ParenthesisOpen),     "(", SourceLocation("ABC", 1, 2), SourceLocation("ABC", 1, 3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ParenthesisClose),    ")", SourceLocation("ABC", 1, 3), SourceLocation("ABC", 1, 4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::CurlyBracketOpen),    "{", SourceLocation("ABC", 1, 4), SourceLocation("ABC", 1, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::CurlyBracketClose),   "}", SourceLocation("ABC", 1, 5), SourceLocation("ABC", 1, 6)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, String)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(
        "\"\"\n\"\\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}\"\n\"${CMAKE_SOURCE_DIR}/cmake\"");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::String), "\"\"", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 1, 3), SourceLocation("ABC", 2, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::String), "\"\\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}\"", SourceLocation("ABC", 2, 1), SourceLocation("ABC", 2, 49)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 2, 49), SourceLocation("ABC", 3, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::String), "\"${CMAKE_SOURCE_DIR}/cmake\"", SourceLocation("ABC", 3, 1), SourceLocation("ABC", 3, 28)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, ReservedKeywordsCaseInsensitive)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(
        "ADD_SUBDIRECTORY\nCMAKE_MINIMUM_REQUIRED\nELSE\nENDFOREACH\n"
        "ENDIF\nFIND_PACKAGE\nFOREACH\nIF\nINCLUDE\nLIST\nMESSAGE\nOPTION\nPROJECT\nSET\nSTRING");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "ADD_SUBDIRECTORY",         SourceLocation("ABC",  1,  1), SourceLocation("ABC",  1,  17)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  1, 17), SourceLocation("ABC",  2,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "CMAKE_MINIMUM_REQUIRED",   SourceLocation("ABC",  2,  1), SourceLocation("ABC",  2, 23)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  2, 23), SourceLocation("ABC",  3,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "ELSE",                     SourceLocation("ABC",  3,  1), SourceLocation("ABC",  3,  5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  3,  5), SourceLocation("ABC",  4,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "ENDFOREACH",               SourceLocation("ABC",  4,  1), SourceLocation("ABC",  4, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  4, 11), SourceLocation("ABC",  5,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "ENDIF",                    SourceLocation("ABC",  5,  1), SourceLocation("ABC",  5,  6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  5,  6), SourceLocation("ABC",  6,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "FIND_PACKAGE",             SourceLocation("ABC",  6,  1), SourceLocation("ABC",  6, 13)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  6, 13), SourceLocation("ABC",  7,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "FOREACH",                  SourceLocation("ABC",  7,  1), SourceLocation("ABC",  7,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  7,  8), SourceLocation("ABC",  8,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "IF",                       SourceLocation("ABC",  8,  1), SourceLocation("ABC",  8,  3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  8,  3), SourceLocation("ABC",  9,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "INCLUDE",                  SourceLocation("ABC",  9,  1), SourceLocation("ABC",  9,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC",  9,  8), SourceLocation("ABC", 10,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "LIST",                     SourceLocation("ABC", 10,  1), SourceLocation("ABC", 10,  5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC", 10,  5), SourceLocation("ABC", 11,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "MESSAGE",                  SourceLocation("ABC", 11,  1), SourceLocation("ABC", 11,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC", 11,  8), SourceLocation("ABC", 12,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "OPTION",                   SourceLocation("ABC", 12,  1), SourceLocation("ABC", 12,  7)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC", 12,  7), SourceLocation("ABC", 13,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "PROJECT",                  SourceLocation("ABC", 13,  1), SourceLocation("ABC", 13,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC", 13,  8), SourceLocation("ABC", 14,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "SET",                      SourceLocation("ABC", 14,  1), SourceLocation("ABC", 14,  4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n",                       SourceLocation("ABC", 14,  4), SourceLocation("ABC", 15,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "STRING",                   SourceLocation("ABC", 15,  1), SourceLocation("ABC", 15,  7)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, ReservedKeywords)
{
    std::string compilationUnit("ABC");
    std::istringstream stream(
        "add_subdirectory\ncmake_minimum_required\nelse\nendforeach\n"
        "endif\nfind_package\nforeach\nif\ninclude\nlist\nmessage\noption\nproject\nset\nstring");
    Lexer lexer(compilationUnit, stream);

    EXPECT_FALSE(lexer.IsAtEnd());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "add_subdirectory", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 17)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 1, 17), SourceLocation("ABC", 2, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "cmake_minimum_required", SourceLocation("ABC", 2, 1), SourceLocation("ABC", 2, 23)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 2, 23), SourceLocation("ABC", 3, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "else", SourceLocation("ABC", 3, 1), SourceLocation("ABC", 3, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 3, 5), SourceLocation("ABC", 4, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "endforeach", SourceLocation("ABC", 4, 1), SourceLocation("ABC", 4, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 4, 11), SourceLocation("ABC", 5, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "endif", SourceLocation("ABC", 5, 1), SourceLocation("ABC", 5, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 5, 6), SourceLocation("ABC", 6, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "find_package", SourceLocation("ABC", 6, 1), SourceLocation("ABC", 6, 13)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 6, 13), SourceLocation("ABC", 7, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "foreach", SourceLocation("ABC", 7, 1), SourceLocation("ABC", 7, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 7, 8), SourceLocation("ABC", 8, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "if", SourceLocation("ABC", 8, 1), SourceLocation("ABC", 8, 3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 8, 3), SourceLocation("ABC", 9, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "include", SourceLocation("ABC", 9, 1), SourceLocation("ABC", 9, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 9, 8), SourceLocation("ABC", 10, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "list", SourceLocation("ABC", 10, 1), SourceLocation("ABC", 10, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 10, 5), SourceLocation("ABC", 11, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "message", SourceLocation("ABC", 11, 1), SourceLocation("ABC", 11, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 11, 8), SourceLocation("ABC", 12, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "option", SourceLocation("ABC", 12, 1), SourceLocation("ABC", 12, 7)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 12, 7), SourceLocation("ABC", 13, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "project", SourceLocation("ABC", 13, 1), SourceLocation("ABC", 13, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 13, 8), SourceLocation("ABC", 14, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "set", SourceLocation("ABC", 14, 1), SourceLocation("ABC", 14, 4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),    "\n", SourceLocation("ABC", 14, 4), SourceLocation("ABC", 15, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::Identifier), "string", SourceLocation("ABC", 15, 1), SourceLocation("ABC", 15, 7)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

//TEST_F(LexerTest, CMakeFileSimpleProject)
//{
//    std::string compilationUnit("CMakeLists_simpleproject.txt");
//    std::filesystem::path inputPath(TEST_DATA_DIR);
//    std::ifstream stream(inputPath / compilationUnit);
//    Lexer lexer(compilationUnit, stream);
//
//    while (!lexer.IsAtEnd())
//    {
//        auto token = lexer.GetToken();
//        //std::cout << "Token " << token.Serialize() << std::endl;
//        if (token.IsInvalid() || token.IsNull())
//            std::cout << "Unexpected token " << token.Serialize() << std::endl;
//    }
//}
//
//TEST_F(LexerTest, CMakeFileRoot)
//{
//    std::string compilationUnit("CMakeLists_root.txt");
//    std::filesystem::path inputPath(TEST_DATA_DIR);
//    std::ifstream stream(inputPath / compilationUnit);
//    Lexer lexer(compilationUnit, stream);
//
//    while (!lexer.IsAtEnd())
//    {
//        auto token = lexer.GetToken();
//        if (token.IsInvalid() || token.IsNull())
//            std::cout << "Unexpected token " << token.Serialize() << std::endl;
//    }
//}
//
//TEST_F(LexerTest, CMakeFileCode)
//{
//    std::string compilationUnit("CMakeLists_code.txt");
//    std::filesystem::path inputPath(TEST_DATA_DIR);
//    std::ifstream stream(inputPath / compilationUnit);
//    Lexer lexer(compilationUnit, stream);
//
//    while (!lexer.IsAtEnd())
//    {
//        auto token = lexer.GetToken();
//        if (token.IsInvalid() || token.IsNull())
//            std::cout << "Unexpected token " << token.Serialize() << std::endl;
//    }
//
//}

} // namespace cmake_parser
