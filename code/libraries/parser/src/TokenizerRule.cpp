#include "parser/TokenizerRule.h"

namespace parser {

static std::string ConvertRegexCaseInsensitive(const std::string& regex)
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

TokenizerRule::TokenizerRule(const std::string& regex, const TokenType& type, bool caseInsensitive /*= false*/)
    : m_regex(caseInsensitive ? ConvertRegexCaseInsensitive(regex): regex)
    , m_type(type)
{
}

bool TokenizerRule::Matches(const std::string& text) const
{
    return std::regex_match(text, m_regex);
}

static TokenizerRules s_tokenizerRules;

void SetTokenizerRules(const TokenizerRules& rules)
{
    s_tokenizerRules = rules;
}

const TokenizerRules& GetTokenizerRules()
{
    return s_tokenizerRules;
}

} // namespace parser
