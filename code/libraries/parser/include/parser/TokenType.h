#pragma once

#include <ostream>
#include <utility>
#include <vector>
#include "serialization/BidirectionalMap.h"

namespace parser {

template<typename UnderlyingType>
class TokenType
{
public:
    template<typename UnderlyingType>
    using TokenDefinition = std::pair<TokenType<UnderlyingType>, std::string>;
    template<typename UnderlyingType>
    using TokenDefinitions = std::vector<TokenDefinition<UnderlyingType>>;
    template<typename UnderlyingType>
    friend void SetupTokenDefinitions(const TokenDefinitions<UnderlyingType>& definitions);
    template<typename UnderlyingType>
    friend TokenType<UnderlyingType> StringToTokenType(const std::string& text);
    static const TokenType InvalidToken;
    static const TokenType NullToken;

private:
    static serialization::BidirectionalMap<TokenType, std::string, std::less<TokenType>> terminalDefinitions;
    UnderlyingType m_type;
    bool m_isInvalid;
    bool m_isNull;

public:
    constexpr TokenType()
        : m_type{}
        , m_isInvalid{}
        , m_isNull{ true }
    {}
    constexpr TokenType(UnderlyingType type, bool isInvalid = false)
        : m_type{ type }
        , m_isInvalid{ isInvalid }
        , m_isNull{}
    {}

    UnderlyingType TypeCode() const { return m_type; }
    constexpr bool Equals(UnderlyingType value) const { return (m_type == value) && !m_isNull && !m_isInvalid; }
    constexpr bool Equals(const TokenType& value) const { return (m_type == value.m_type) && (m_isNull == value.m_isNull) && (m_isInvalid == value.m_isInvalid); }
    constexpr bool operator <(const TokenType& rhs) const
    {
        return m_type < rhs.m_type;
    }
    std::string Serialize() const
    {
        if (m_isInvalid)
        {
            return "Invalid";
        }
        if (m_isNull)
        {
            return "Null";
        }
        return terminalDefinitions.Translate(m_type);
    }
};

template<typename UnderlyingType>
const TokenType<UnderlyingType> TokenType<UnderlyingType>::InvalidToken{ UnderlyingType{}, true };

template<typename UnderlyingType>
const TokenType<UnderlyingType> TokenType<UnderlyingType>::NullToken{};

template<typename UnderlyingType>
serialization::BidirectionalMap<TokenType<UnderlyingType>, std::string, std::less<TokenType<UnderlyingType>>> TokenType<UnderlyingType>::terminalDefinitions{};

template<typename UnderlyingType>
inline bool operator ==(const TokenType<UnderlyingType>& lhs, UnderlyingType rhs) { return lhs.Equals(rhs); }
template<typename UnderlyingType>
inline bool operator ==(UnderlyingType lhs, const TokenType<UnderlyingType>& rhs) { return rhs.Equals(lhs); }
template<typename UnderlyingType>
inline bool operator ==(const TokenType<UnderlyingType>& lhs, const TokenType<UnderlyingType>& rhs) { return lhs.Equals(rhs); }

template<typename UnderlyingType>
inline bool operator !=(const TokenType<UnderlyingType>& lhs, UnderlyingType rhs) { return !(lhs == rhs); }
template<typename UnderlyingType>
inline bool operator !=(UnderlyingType lhs, const TokenType<UnderlyingType>& rhs) { return !(lhs == rhs); }
template<typename UnderlyingType>
inline bool operator !=(const TokenType<UnderlyingType>& lhs, const TokenType<UnderlyingType>& rhs) { return !(lhs == rhs); }

template<typename UnderlyingType>
using TokenDefinition = std::pair<TokenType<UnderlyingType>, std::string>;
template<typename UnderlyingType>
using TokenDefinitions = std::vector<TokenDefinition<UnderlyingType>>;

template<typename UnderlyingType>
void SetupTokenDefinitions(const TokenDefinitions<UnderlyingType>& definitions)
{
    TokenType<UnderlyingType>::terminalDefinitions.Init(definitions);
}

template<typename UnderlyingType>
TokenType<UnderlyingType> StringToTokenType(const std::string& text)
{
    return TokenType<UnderlyingType>::terminalDefinitions.Translate(text);
}

template<typename UnderlyingType>
inline std::ostream& operator << (std::ostream& stream, const TokenType<UnderlyingType>& value)
{
    return stream << value.Serialize();
}

} // namespace parser
