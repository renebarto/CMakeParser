#pragma once

#include "parser/Token.h"

#include <regex>

namespace parser {

template<typename UnderlyingType>
class TokenizerRule
{
public:
    static std::vector<TokenizerRule<UnderlyingType>> tokenizerRules;

    std::string ConvertRegexCaseInsensitive(const std::string& regex)
    {
        std::string result;
        for (auto c : regex)
        {
            if (isalpha(c))
            {
                result += "[";
                result += static_cast<char>(tolower(c));
                result += static_cast<char>(toupper(c));
                result += "]";
            }
            else
            {
                result += c;
            }
        }
        return result;
    }

    TokenizerRule(const std::string& regex, const TokenType<UnderlyingType>& type, bool caseInsensitive = false)
        : m_regex(caseInsensitive ? ConvertRegexCaseInsensitive(regex) : regex)
        , m_type(type)
    {
    }

    bool Matches(const std::string& text) const
    {
        return std::regex_match(text, m_regex);
    }
    TokenType<UnderlyingType> Type() const { return m_type; }

private:
    std::regex m_regex;
    TokenType<UnderlyingType> m_type;
};

template<typename UnderlyingType>
using TokenizerRules = std::vector<TokenizerRule<UnderlyingType>>;

template<typename UnderlyingType>
std::vector<TokenizerRule<UnderlyingType>> TokenizerRule<UnderlyingType>::tokenizerRules{};

template<typename UnderlyingType>
void SetTokenizerRules(const TokenizerRules<UnderlyingType>& rules)
{
    TokenizerRule<UnderlyingType>::tokenizerRules = rules;
}

template<typename UnderlyingType>
const TokenizerRules<UnderlyingType>& GetTokenizerRules()
{
    return TokenizerRule<UnderlyingType>::tokenizerRules;
}

} // namespace parser
