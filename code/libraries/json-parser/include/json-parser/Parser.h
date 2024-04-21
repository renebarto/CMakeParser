#pragma once

#include <memory>
#include <set>
#include "json-parser/Lexer.h"
#include "json-parser/JSONValue.h"

namespace json_parser {

using JSONValuePtr = std::shared_ptr<JSONValue>;

class Parser
{
private:
    Lexer m_lexer;
    JSONValuePtr m_value;
    int m_errorCount;

public:
    Parser(const std::string& path, std::istream& stream);
    bool Parse();

    JSONValuePtr GetValue();
    const JSONValuePtr GetValue() const;

    JSONValuePtr ParseValue();
    JSONValuePtr ParseArray();
    JSONValuePtr ParseObject();

    bool Expect(TokenTypes type, parser::Token<TokenTypes>& token);
    bool Expect(std::set<TokenTypes> oneOfTypes, parser::Token<TokenTypes>& token);

    void OnNoMoreToken(const parser::SourceLocation& location);
    void OnParseError(const std::string& text, const parser::SourceLocation& startLocation, const parser::SourceLocation& endLocation);
};

} // namespace json_parser
