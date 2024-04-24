#pragma once

#include<filesystem>
#include <iostream>
#include <vector>
#include "SourceLocation.h"

namespace parser {

class Reader
{
private:
    std::filesystem::path m_unitPath;
    std::istream & m_stream;
    std::string m_buffer;
    bool m_endOfBuffer;
    SourceLocation m_currentLocation;

public:
    Reader(const std::filesystem::path& unitPath, std::istream& stream);
    bool GetChar(char & ch);
    void RestoreChars(const std::string& restoreBuffer, const SourceLocation& restoreLocation);
    const SourceLocation GetLocation() const { return m_currentLocation; }
    bool EndOfStream() const;

private:
    void Advance(char c);
};

} // namespace parser