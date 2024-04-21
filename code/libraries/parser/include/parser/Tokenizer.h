#pragma once

#include <memory>
#include <stack>
#include "parser/ITokenizer.h"
#include "parser/Reader.h"
#include "parser/Token.h"
#include "parser/TokenizerRule.h"

namespace parser {

template<typename UnderlyingType>
class Tokenizer
    : public ITokenizer<UnderlyingType>
{
private:
    Reader m_reader;
    std::stack<Token<UnderlyingType>> m_tokenBuffer;

public:
    Tokenizer(const std::string& compilationUnit, std::istream& stream)
        : m_reader(compilationUnit, stream)
    {
    }

    ~Tokenizer()
    {
    }

    Token<UnderlyingType> GetToken();
    void UngetToken(const Token<UnderlyingType>& token)
    {
        m_tokenBuffer.push(token);
    }
    SourceLocation GetCurrentLocation() const
    {
        return m_reader.GetLocation();
    }
    bool IsAtEnd() const
    {
        return m_reader.EndOfStream() && m_tokenBuffer.empty();
    }
    typename TokenizerRules<UnderlyingType>::const_iterator FirstMatch(const std::string& text)
    {
        auto it = GetTokenizerRules<UnderlyingType>().begin();
        while (it != GetTokenizerRules<UnderlyingType>().end())
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
        auto it = GetTokenizerRules<UnderlyingType>().begin();
        while (it != GetTokenizerRules<UnderlyingType>().end())
        {
            if (it->Matches(text))
                numMatches++;
            ++it;
        }
        return numMatches > 1;
    }
};

template<typename UnderlyingType>
using TokenizerPtr = std::unique_ptr<Tokenizer<UnderlyingType>>;

template<typename UnderlyingType>
Token<UnderlyingType> Tokenizer<UnderlyingType>::GetToken()
{
    char ch;
    Token<UnderlyingType> token;

    if (!m_tokenBuffer.empty())
    {
        token = m_tokenBuffer.top();
        m_tokenBuffer.pop();
        return token;
    }

    std::string currentTerm;
    std::string extendedTerm;
    SourceLocation startLocation = m_reader.GetLocation();
    TokenizerRules<UnderlyingType>::const_iterator firstMatch = GetTokenizerRules<UnderlyingType>().end();
    TokenizerRules<UnderlyingType>::const_iterator lastMatch = GetTokenizerRules<UnderlyingType>().end();
    TokenizerRules<UnderlyingType>::const_iterator match = GetTokenizerRules<UnderlyingType>().end();
    SourceLocation lastLocation{};
    const size_t MaxLookAheadCharacters = 10;
    size_t lookAheadCharacters{};

    while (m_reader.GetChar(ch))
    {
        firstMatch = FirstMatch(currentTerm + extendedTerm + ch);
        if (firstMatch == GetTokenizerRules<UnderlyingType>().end())
        {
            if (match != GetTokenizerRules<UnderlyingType>().end())
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
                if (firstMatch == GetTokenizerRules<UnderlyingType>().end())
                {
                    m_reader.RestoreChars(extendedTerm, lastLocation);
                    extendedTerm = {};
                    firstMatch = lastMatch;
                    break;
                }
            }
            else if (firstMatch != GetTokenizerRules<UnderlyingType>().end())
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
    if (match != GetTokenizerRules<UnderlyingType>().end())
    {
        token = Token(match->Type(), currentTerm, startLocation, lastLocation);
    }
    if (token.IsNull() && !currentTerm.empty())
    {
        token = Token(TokenType<UnderlyingType>::InvalidToken, currentTerm, startLocation, m_reader.GetLocation());
    }
    return token;
}

} // namespace parser
