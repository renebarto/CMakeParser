#include "cmake-parser/Lexer.h"

#include "gtest/gtest.h"

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
    EXPECT_EQ(Token(), lexer.GetToken());
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
    EXPECT_EQ(Token(TokenType(Terminal::AddSubDirectoryKeyword),        "ADD_SUBDIRECTORY",         SourceLocation("ABC",  1,  1), SourceLocation("ABC",  1,  17)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  1, 17), SourceLocation("ABC",  2,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::CMakeMinimumRequiredKeyword),   "CMAKE_MINIMUM_REQUIRED",   SourceLocation("ABC",  2,  1), SourceLocation("ABC",  2, 23)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  2, 23), SourceLocation("ABC",  3,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ElseKeyword),                   "ELSE",                     SourceLocation("ABC",  3,  1), SourceLocation("ABC",  3,  5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  3,  5), SourceLocation("ABC",  4,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::EndForEachKeyword),             "ENDFOREACH",               SourceLocation("ABC",  4,  1), SourceLocation("ABC",  4, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  4, 11), SourceLocation("ABC",  5,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::EndIfKeyword),                  "ENDIF",                    SourceLocation("ABC",  5,  1), SourceLocation("ABC",  5,  6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  5,  6), SourceLocation("ABC",  6,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::FindPackageKeyword),            "FIND_PACKAGE",             SourceLocation("ABC",  6,  1), SourceLocation("ABC",  6, 13)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  6, 13), SourceLocation("ABC",  7,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ForEachKeyword),                "FOREACH",                  SourceLocation("ABC",  7,  1), SourceLocation("ABC",  7,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  7,  8), SourceLocation("ABC",  8,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::IfKeyword),                     "IF",                       SourceLocation("ABC",  8,  1), SourceLocation("ABC",  8,  3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  8,  3), SourceLocation("ABC",  9,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::IncludeKeyword),                "INCLUDE",                  SourceLocation("ABC",  9,  1), SourceLocation("ABC",  9,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC",  9,  8), SourceLocation("ABC", 10,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ListKeyword),                   "LIST",                     SourceLocation("ABC", 10,  1), SourceLocation("ABC", 10,  5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC", 10,  5), SourceLocation("ABC", 11,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::MessageKeyword),                "MESSAGE",                  SourceLocation("ABC", 11,  1), SourceLocation("ABC", 11,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC", 11,  8), SourceLocation("ABC", 12,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::OptionKeyword),                 "OPTION",                   SourceLocation("ABC", 12,  1), SourceLocation("ABC", 12,  7)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC", 12,  7), SourceLocation("ABC", 13,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ProjectKeyword),                "PROJECT",                  SourceLocation("ABC", 13,  1), SourceLocation("ABC", 13,  8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC", 13,  8), SourceLocation("ABC", 14,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::SetKeyword),                    "SET",                      SourceLocation("ABC", 14,  1), SourceLocation("ABC", 14,  4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine),                       "\n",                       SourceLocation("ABC", 14,  4), SourceLocation("ABC", 15,  1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::StringKeyword),                 "STRING",                   SourceLocation("ABC", 15,  1), SourceLocation("ABC", 15,  7)), lexer.GetToken());
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
    EXPECT_EQ(Token(TokenType(Terminal::AddSubDirectoryKeyword), "add_subdirectory", SourceLocation("ABC", 1, 1), SourceLocation("ABC", 1, 17)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 1, 17), SourceLocation("ABC", 2, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::CMakeMinimumRequiredKeyword), "cmake_minimum_required", SourceLocation("ABC", 2, 1), SourceLocation("ABC", 2, 23)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 2, 23), SourceLocation("ABC", 3, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ElseKeyword), "else", SourceLocation("ABC", 3, 1), SourceLocation("ABC", 3, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 3, 5), SourceLocation("ABC", 4, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::EndForEachKeyword), "endforeach", SourceLocation("ABC", 4, 1), SourceLocation("ABC", 4, 11)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 4, 11), SourceLocation("ABC", 5, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::EndIfKeyword), "endif", SourceLocation("ABC", 5, 1), SourceLocation("ABC", 5, 6)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 5, 6), SourceLocation("ABC", 6, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::FindPackageKeyword), "find_package", SourceLocation("ABC", 6, 1), SourceLocation("ABC", 6, 13)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 6, 13), SourceLocation("ABC", 7, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ForEachKeyword), "foreach", SourceLocation("ABC", 7, 1), SourceLocation("ABC", 7, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 7, 8), SourceLocation("ABC", 8, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::IfKeyword), "if", SourceLocation("ABC", 8, 1), SourceLocation("ABC", 8, 3)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 8, 3), SourceLocation("ABC", 9, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::IncludeKeyword), "include", SourceLocation("ABC", 9, 1), SourceLocation("ABC", 9, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 9, 8), SourceLocation("ABC", 10, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ListKeyword), "list", SourceLocation("ABC", 10, 1), SourceLocation("ABC", 10, 5)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 10, 5), SourceLocation("ABC", 11, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::MessageKeyword), "message", SourceLocation("ABC", 11, 1), SourceLocation("ABC", 11, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 11, 8), SourceLocation("ABC", 12, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::OptionKeyword), "option", SourceLocation("ABC", 12, 1), SourceLocation("ABC", 12, 7)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 12, 7), SourceLocation("ABC", 13, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::ProjectKeyword), "project", SourceLocation("ABC", 13, 1), SourceLocation("ABC", 13, 8)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 13, 8), SourceLocation("ABC", 14, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::SetKeyword), "set", SourceLocation("ABC", 14, 1), SourceLocation("ABC", 14, 4)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::NewLine), "\n", SourceLocation("ABC", 14, 4), SourceLocation("ABC", 15, 1)), lexer.GetToken());
    EXPECT_EQ(Token(TokenType(Terminal::StringKeyword), "string", SourceLocation("ABC", 15, 1), SourceLocation("ABC", 15, 7)), lexer.GetToken());
    EXPECT_TRUE(lexer.IsAtEnd());
}

TEST_F(LexerTest, CMakeFileSimpleProject)
{
    std::string compilationUnit("CMakeLists_simpleproject.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Lexer lexer(compilationUnit, stream);

    while (!lexer.IsAtEnd())
    {
        auto token = lexer.GetToken();
        if (token.IsInvalid() || token.IsNull())
            std::cout << "Unexpected token " << token.Serialize() << std::endl;
    }
}

TEST_F(LexerTest, CMakeFileRoot)
{
    std::string compilationUnit("CMakeLists_root.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Lexer lexer(compilationUnit, stream);

    while (!lexer.IsAtEnd())
    {
        auto token = lexer.GetToken();
        if (token.IsInvalid() || token.IsNull())
            std::cout << "Unexpected token " << token.Serialize() << std::endl;
    }
}

TEST_F(LexerTest, CMakeFileCode)
{
    std::string compilationUnit("CMakeLists_code.txt");
    std::filesystem::path inputPath(TEST_DATA_DIR);
    std::ifstream stream(inputPath / compilationUnit);
    Lexer lexer(compilationUnit, stream);

    while (!lexer.IsAtEnd())
    {
        auto token = lexer.GetToken();
        if (token.IsInvalid() || token.IsNull())
            std::cout << "Unexpected token " << token.Serialize() << std::endl;
    }

}

} // namespace cmake_parser
