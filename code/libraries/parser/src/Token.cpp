#include "parser/Token.h"

#include <sstream>

namespace parser {

Token::Token()
    : m_type{}
    , m_value{}
{
}

Token::Token(const TokenType& type, const std::string& value, const SourceLocation& beginLocation, const SourceLocation& endLocation)
    : m_type{ type }
    , m_value{ value }
    , m_startLocation{ beginLocation }
    , m_endLocation{ endLocation }
{
}

bool Token::IsNull() const
{
    return m_type == TokenType{};
}

bool Token::IsInvalid() const
{
    return m_type == TokenType::InvalidToken;
}

TokenType Token::Type() const
{
    return m_type;
}

std::string Token::Value() const
{
    return m_value;
}

SourceLocation Token::Location() const
{
    return m_startLocation;
}

SourceLocation Token::BeginLocation() const
{
    return m_startLocation;
}

SourceLocation Token::EndLocation() const
{
    return m_endLocation;
}

std::string Token::Serialize() const
{
    std::ostringstream stream;
    stream << m_type << " " << m_value << " (" << m_startLocation << "-" << m_endLocation << ")";
    return stream.str();
}

bool Token::Equals(const Token& other) const
{
    return 
        (m_type == other.m_type) && (m_value == other.m_value) && 
        (m_startLocation == other.m_startLocation) && (m_endLocation == other.m_endLocation);
}

bool Token::Equals(const TokenType& tokenType) const
{
    return m_type == tokenType;
}

} // namespace parser
