#pragma once

#include "parser/Token.h"

#include <regex>

namespace parser {

class TokenizerRule
{
public:
    TokenizerRule(const std::string& regex, const TokenType& type, bool caseInsensitive = false);

    bool Matches(const std::string& text) const;
    TokenType Type() const { return m_type; }

private:
    std::regex m_regex;
    TokenType m_type;
};

using TokenizerRules = std::vector<TokenizerRule>;

void SetTokenizerRules(const TokenizerRules& rules);
const TokenizerRules& GetTokenizerRules();

} // namespace parser
