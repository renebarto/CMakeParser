#pragma once

#include <functional>
#include <memory>
#include "parser/IParserCallback.h"
#include "parser/ITokenizer.h"

namespace parser {

template<typename UnderlyingType>
class ParserExecutor
{
private:
    IParserCallback<UnderlyingType>& m_parserCallback;
    std::set<TokenType<UnderlyingType>> m_skipWhitespaceTokens;

public:
    ParserExecutor(IParserCallback<UnderlyingType>& parserCallback, std::set<TokenType<UnderlyingType>> skipWhiteSpaceTokens)
        : m_parserCallback{ parserCallback }
        , m_skipWhitespaceTokens{ skipWhiteSpaceTokens }
    {}
    bool Parse(ITokenizer<UnderlyingType>& tokenizer)
    {
        bool result{ true };
        bool done{ false };
        while (result && !done)
        {
            Token token = tokenizer.GetToken();
            if (token.IsNull())
            {
                result = m_parserCallback.OnNoMoreToken(tokenizer.GetCurrentLocation());
                break;
            }
            if (token.IsInvalid())
            {
                m_parserCallback.OnParseError(token);
                result = false;
                break;
            }
            if (m_skipWhitespaceTokens.find(token.Type()) != m_skipWhitespaceTokens.end())
            {
                m_parserCallback.OnSkipToken(token);
                continue;
            }
            if (!m_parserCallback.OnToken(token, done))
            {
                m_parserCallback.OnParseError(token);
                result = false;
            }
            if (done)
                tokenizer.UngetToken(token);
        }
        return result;
    }
};

template<typename UnderlyingType>
using ParserExecutorPtr = std::unique_ptr<ParserExecutor<UnderlyingType>>;

} // namespace parser
