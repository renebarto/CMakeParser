#pragma once

#include "parser/Token.h"

namespace parser {

template<typename UnderlyingType>
class ITokenizer
{
public:
    virtual ~ITokenizer() = default;

    virtual Token<UnderlyingType> GetToken() = 0;
    virtual void UngetToken(const Token<UnderlyingType>& token) = 0;
    virtual SourceLocation GetCurrentLocation() const = 0;
    virtual bool IsAtEnd() const = 0;
};

} // namespace parser
