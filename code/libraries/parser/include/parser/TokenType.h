#pragma once

#include <ostream>
#include <utility>
#include <vector>

namespace parser {

class TokenType
{
public:
    static const TokenType InvalidToken;
    int m_type;
    constexpr TokenType() : m_type{} {}
    constexpr TokenType(int type) : m_type{type} {}

    constexpr bool Equals(int value) const { return m_type == value; }
    constexpr bool Equals(const TokenType& value) const { return m_type == value.m_type; }
    constexpr bool operator <(const TokenType& rhs) const
    {
        return m_type < rhs.m_type;
    }
    std::string Serialize() const;
};

inline bool operator ==(const TokenType& lhs, int rhs) { return lhs.Equals(rhs); }
inline bool operator ==(int lhs, const TokenType& rhs) { return rhs.Equals(lhs); }
inline bool operator ==(const TokenType& lhs, const TokenType& rhs) { return lhs.Equals(rhs); }

inline bool operator !=(const TokenType& lhs, int rhs) { return !(lhs == rhs); }
inline bool operator !=(int lhs, const TokenType& rhs) { return !(lhs == rhs); }
inline bool operator !=(const TokenType& lhs, const TokenType& rhs) { return !(lhs == rhs); }

using TokenDefinition = std::pair<TokenType, std::string>;
using TokenDefinitions = std::vector<TokenDefinition>;

void SetupTokenDefinitions(const TokenDefinitions& definitions);
TokenType StringToTokenType(const std::string& text);

inline std::ostream& operator << (std::ostream& stream, const TokenType& value)
{
    return stream << value.Serialize();
}

} // namespace parser
