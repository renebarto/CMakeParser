#pragma once

#include <sstream>
#include <string>
#include "SourceLocation.h"
#include "TokenType.h"

namespace parser
{

template<typename UnderlyingType>
class Token
{
private:
    TokenType<UnderlyingType> m_type;
    std::string m_value;
    SourceLocation m_startLocation;
    SourceLocation m_endLocation;

public:
    Token()
        : m_type{}
        , m_value{}
        , m_startLocation{}
        , m_endLocation{}
    {
    }
    Token(const TokenType<UnderlyingType>& type, const std::string& value, const SourceLocation& beginLocation, const SourceLocation& endLocation)
        : m_type{ type }
        , m_value{ value }
        , m_startLocation{ beginLocation }
        , m_endLocation{ endLocation }
    {
    }

    bool IsNull() const { return m_type == TokenType<UnderlyingType>{}; }
    bool IsInvalid() const { return m_type == TokenType<UnderlyingType>::InvalidToken; }
    TokenType<UnderlyingType> Type() const { return m_type; }
    std::string Value() const { return m_value; }
    SourceLocation Location() const { return m_startLocation; }
    SourceLocation BeginLocation() const { return m_startLocation; }
    SourceLocation EndLocation() const { return m_endLocation; }
    std::string Serialize() const
    {
        std::ostringstream stream;
        stream << m_type << " " << m_value << " (" << m_startLocation << "-" << m_endLocation << ")";
        return stream.str();
    }
    bool Equals(const Token& other) const
    {
        return
            (m_type == other.m_type) && (m_value == other.m_value) &&
            (m_startLocation == other.m_startLocation) && (m_endLocation == other.m_endLocation);
    }
    bool Equals(const TokenType<UnderlyingType>& tokenType) const
    {
        return m_type == tokenType;
    }
};

template<typename UnderlyingType>
inline bool operator ==(const Token<UnderlyingType>& lhs, const TokenType<UnderlyingType>& rhs) { return lhs.Equals(rhs); }
template<typename UnderlyingType>
inline bool operator ==(const TokenType<UnderlyingType>& lhs, const Token<UnderlyingType>& rhs) { return rhs.Equals(lhs); }
template<typename UnderlyingType>
inline bool operator ==(const Token<UnderlyingType>& lhs, const Token<UnderlyingType>& rhs) { return lhs.Equals(rhs); }

template<typename UnderlyingType>
inline bool operator !=(const Token<UnderlyingType>& lhs, const TokenType<UnderlyingType>& rhs) { return !(lhs == rhs); }
template<typename UnderlyingType>
inline bool operator !=(const TokenType<UnderlyingType>& lhs, const Token<UnderlyingType>& rhs) { return !(lhs == rhs); }
template<typename UnderlyingType>
inline bool operator !=(const Token<UnderlyingType>& lhs, const Token<UnderlyingType>& rhs) { return !(lhs == rhs); }

template<typename UnderlyingType>
using TokenList = std::vector<Token<UnderlyingType>>;

template<typename UnderlyingType>
inline std::ostream& operator << (std::ostream& stream, const Token<UnderlyingType>& value)
{
    return stream << value.Serialize();
}

} // namespace parser
