#include "parser/SourceLocation.h"

#include <string>

namespace parser {

SourceLocation::SourceLocation()
    : m_line{}
    , m_col{}
{
}

SourceLocation::SourceLocation(const std::filesystem::path& unitPath)
    : m_unitPath{ unitPath }
    , m_line{}
    , m_col{}
{
}

SourceLocation::SourceLocation(const std::filesystem::path& unitPath, int line, int column)
    : m_unitPath{ unitPath }
    , m_line{ line }
    , m_col{ column }
{
}

void SourceLocation::NextCol()
{
    ++m_col;
}

void SourceLocation::ResetLine()
{
    m_col = 1;
}

void SourceLocation::NextLine()
{
    ++m_line;
    ResetLine();
}

bool SourceLocation::IsValid() const
{
    return (m_line != 0) && (m_col != 0);
}

std::string SourceLocation::Serialize() const
{
    if (!IsValid())
        return "??";
    return m_unitPath.generic_string() + "(" + std::to_string(m_line) + ":" + std::to_string(m_col) + ")";
}

bool operator ==(const SourceLocation& lhs, const SourceLocation& rhs)
{
    return (lhs.Line() == rhs.Line()) && (lhs.Column() == rhs.Column());
}

bool operator !=(const SourceLocation& lhs, const SourceLocation& rhs)
{
    return !operator ==(lhs, rhs);
}

std::ostream& operator << (std::ostream& stream, const SourceLocation& value)
{
    return stream << value.Serialize();
}

} // namespace parser