#include "parser/IParserCallback.h"

#include "test-platform/GoogleMock.h"

namespace parser {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::NaggyMock;

template<typename UnderlyingType>
class ParserCallbackMock
    : public IParserCallback<UnderlyingType>
{
public:
    MOCK_METHOD(bool, OnToken, (const Token<UnderlyingType>& token, bool& done), (override));
    MOCK_METHOD(bool, OnNoMoreToken, (const SourceLocation& location), (override));
    MOCK_METHOD(void, OnSkipToken, (const Token<UnderlyingType>& token), (override));
    MOCK_METHOD(void, OnParseError, (const Token<UnderlyingType>& token), (override));
};

} // namespace parser
