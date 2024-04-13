#pragma once

#include <iostream>
#include <vector>

namespace parser {

class SourceLocation
{
private:
    std::string m_unitPath;
    int m_line;
    int m_col;

public:
    SourceLocation();
    SourceLocation(const std::string& unitPath);
    SourceLocation(const std::string& unitPath, int line, int column);

    void NextCol();
    void NextLine();
    void ResetLine();
    const std::string& UnitPath() const { return m_unitPath; }
    int Line() const { return m_line; }
    int Column() const { return m_col; }
    bool IsValid() const;

    std::string Serialize() const;
};

bool operator ==(const SourceLocation& lhs, const SourceLocation& rhs);
bool operator !=(const SourceLocation& lhs, const SourceLocation& rhs);
std::ostream& operator << (std::ostream& stream, const SourceLocation& value);

} // namespace parser