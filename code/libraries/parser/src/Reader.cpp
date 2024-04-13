#include "parser/Reader.h"

namespace parser {

Reader::Reader(const std::string& unitPath, std::istream& stream)
    : m_unitPath{ unitPath }
    , m_stream(stream)
    , m_buffer{}
    , m_endOfBuffer{}
    , m_currentLocation{}
{
    if (m_stream.good())
    {
        m_currentLocation = SourceLocation(m_unitPath, 1, 1);
    }
}

void Reader::Advance(char ch)
{
    switch (ch)
    {
        case '\n':
            m_currentLocation.NextLine();
            break;
        case '\r':
            m_currentLocation.ResetLine();
            break;
        default:
            m_currentLocation.NextCol();
            break;
    }
}

bool Reader::GetChar(char & ch)
{
    ch = {};
    if (!m_buffer.empty())
    {
        ch = m_buffer[0];
        m_buffer = m_buffer.substr(1);
        Advance(ch);
        return true;
    }
    m_endOfBuffer = true;
    if (m_stream.good())
    {
        if (m_stream.get(ch))
        {
            Advance(ch);
            return true;
        }
    }
    return false;
}

void Reader::RestoreChars(const std::string& restoreBuffer, const SourceLocation& restoreLocation)
{
    m_currentLocation = restoreLocation;
    m_buffer = restoreBuffer + m_buffer;
    m_endOfBuffer = false;
}

bool Reader::EndOfStream() const
{
    return m_endOfBuffer && m_stream.eof();
}

} // namespace parser