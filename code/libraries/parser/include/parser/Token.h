#pragma once

#include <string>
#include "SourceLocation.h"
#include "TokenType.h"

namespace parser
{

class Token
{
private:
    TokenType m_type;
    std::string m_value;
    SourceLocation m_startLocation;
    SourceLocation m_endLocation;

public:
    Token();
    Token(const TokenType& type, const std::string& value, const SourceLocation& beginLocation, const SourceLocation& endLocation);

    bool IsNull() const;
    bool IsInvalid() const;
    TokenType Type() const;
    std::string Value() const;
    SourceLocation Location() const;
    SourceLocation BeginLocation() const;
    SourceLocation EndLocation() const;
    std::string Serialize() const;
    bool Equals(const Token& other) const;
    bool Equals(const TokenType& tokenType) const;
};

inline bool operator ==(const Token& lhs, const TokenType& rhs) { return lhs.Equals(rhs); }
inline bool operator ==(const TokenType& lhs, const Token& rhs) { return rhs.Equals(lhs); }
inline bool operator ==(const Token& lhs, const Token& rhs) { return lhs.Equals(rhs); }

inline bool operator !=(const Token& lhs, const TokenType& rhs) { return !(lhs == rhs); }
inline bool operator !=(const TokenType& lhs, const Token& rhs) { return !(lhs == rhs); }
inline bool operator !=(const Token& lhs, const Token& rhs) { return !(lhs == rhs); }

using TokenList = std::vector<Token>;

inline std::ostream& operator << (std::ostream& stream, const Token& value)
{
    return stream << value.Serialize();
}

} // namespace parser
