#pragma once

#include <functional>
#include "parser/Token.h"

namespace parser {

class SourceLocation;

template<typename UnderlyingType>
class IParserCallback
{
public:
    virtual ~IParserCallback() = default;
    virtual bool OnToken(const Token<UnderlyingType>&, bool& done) = 0;
    virtual bool OnNoMoreToken(const SourceLocation& location) = 0;
    virtual void OnSkipToken(const Token<UnderlyingType>& token) = 0;
    virtual void OnParseError(const Token<UnderlyingType>& token) = 0;
};

} // namespace parser
