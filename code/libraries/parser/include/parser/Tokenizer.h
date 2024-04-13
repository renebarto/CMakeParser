#pragma once

#include <memory>
#include <stack>
#include "parser/Reader.h"
#include "parser/Token.h"

namespace parser {

class Tokenizer
{
private:
    Reader m_reader;
    std::stack<Token> m_tokenBuffer;

public:
    Tokenizer(const std::string& compilationUnit, std::istream& stream);
    ~Tokenizer();

    Token GetToken();
    void UngetToken(const Token& token);
    SourceLocation GetCurrentLocation() const;
    bool IsAtEnd() const;
};

using TokenizerPtr = std::unique_ptr<Tokenizer>;

} // namespace parser
