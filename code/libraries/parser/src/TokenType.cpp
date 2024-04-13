#include "parser/TokenType.h"
#include "serialization/BidirectionalMap.h"

namespace parser {

static serialization::BidirectionalMap<TokenType, std::string, std::less<TokenType>> s_terminalDefinitions;

const TokenType TokenType::InvalidToken(-1);

std::string TokenType::Serialize() const
{
    if (m_type == TokenType::InvalidToken)
    {
        return "Invalid";
    }
    return s_terminalDefinitions.Translate(m_type);
}

void SetupTokenDefinitions(const TokenDefinitions& definitions)
{
    s_terminalDefinitions.Init(definitions);
}

TokenType StringToTokenType(const std::string& text)
{
    return s_terminalDefinitions.Translate(text);
}

} // namespace parser
