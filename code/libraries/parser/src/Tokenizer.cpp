#include "parser/Tokenizer.h"
#include "parser/TokenizerRule.h"

#include <regex>

namespace parser {

TokenizerRules::const_iterator FirstMatch(const std::string& text)
{
    auto it = GetTokenizerRules().begin();
    while (it != GetTokenizerRules().end())
    {
        if (it->Matches(text))
            return it;
        ++it;
    }
    return it;
}

bool HaveMultipleMatches(const std::string& text)
{
    size_t numMatches{};
    auto it = GetTokenizerRules().begin();
    while (it != GetTokenizerRules().end())
    {
        if (it->Matches(text))
            numMatches++;
        ++it;
    }
    return numMatches > 1;
}

Tokenizer::Tokenizer(const std::string& compilationUnit, std::istream& stream)
    : m_reader(compilationUnit, stream)
{
}

Tokenizer::~Tokenizer()
{
}

Token Tokenizer::GetToken()
{
    char ch;
    Token token;

    if (!m_tokenBuffer.empty())
    {
        token = m_tokenBuffer.top();
        m_tokenBuffer.pop();
        return token;
    }

    std::string currentTerm;
    std::string extendedTerm;
    SourceLocation startLocation = m_reader.GetLocation();
    TokenizerRules::const_iterator firstMatch = GetTokenizerRules().end();
    TokenizerRules::const_iterator lastMatch = GetTokenizerRules().end();
    TokenizerRules::const_iterator match = GetTokenizerRules().end();
    SourceLocation lastLocation{};
    const size_t MaxLookAheadCharacters = 10;
    size_t lookAheadCharacters{};

    while (m_reader.GetChar(ch))
    {
        firstMatch = FirstMatch(currentTerm + extendedTerm + ch);
        if (firstMatch == GetTokenizerRules().end())
        {
            if (match != GetTokenizerRules().end())
            {
                if (lookAheadCharacters < MaxLookAheadCharacters)
                {
                    lookAheadCharacters++;
                    extendedTerm += ch;
                    continue;
                }
                else
                {
                    m_reader.RestoreChars(extendedTerm + ch, lastLocation);
                    extendedTerm = {};
                    break;
                }
            }
            else
            {
                currentTerm += ch;
                continue;
            }
        }
        lookAheadCharacters = 0;
        currentTerm += extendedTerm;
        currentTerm += ch;
        extendedTerm = {};
        lastMatch = firstMatch;
        lastLocation = m_reader.GetLocation();
        while (HaveMultipleMatches(currentTerm + extendedTerm))
        {
            if (m_reader.GetChar(ch))
            {
                extendedTerm += ch;
                firstMatch = FirstMatch(currentTerm + extendedTerm);
                if (firstMatch == GetTokenizerRules().end())
                {
                    m_reader.RestoreChars(extendedTerm, lastLocation);
                    extendedTerm = {};
                    firstMatch = lastMatch;
                    break;
                }
            }
            else if (firstMatch != GetTokenizerRules().end())
            {
                currentTerm += extendedTerm;
                extendedTerm = {};
                lastLocation = m_reader.GetLocation();
                break;
            }
        }
        match = firstMatch;
    }
    if (!extendedTerm.empty())
    {
        m_reader.RestoreChars(extendedTerm, lastLocation);
    }
    if (match != GetTokenizerRules().end())
    {
        token = Token(match->Type(), currentTerm, startLocation, lastLocation);
    }
    if (token.IsNull() && !currentTerm.empty())
    {
        token = Token(TokenType::InvalidToken, currentTerm, startLocation, m_reader.GetLocation());
    }
    return token;
}

void Tokenizer::UngetToken(const Token& token)
{
    m_tokenBuffer.push(token);
}

SourceLocation Tokenizer::GetCurrentLocation() const
{
    return m_reader.GetLocation();
}

bool Tokenizer::IsAtEnd() const
{
    return m_reader.EndOfStream() && m_tokenBuffer.empty();
}

} // namespace parser
