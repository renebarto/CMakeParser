#include "utility/Console.h"

#include <map>
#include <string>

namespace utility {

static std::ostream* DetermineStream(FileDescriptor handle)
{
    switch (handle)
    {
    case STDOUT_FILENO:
        return &StdOut;
    case STDERR_FILENO:
        return &StdErr;
    case STDIN_FILENO:
    default:
        StdErr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

static FileDescriptor DetermineHandle(std::ostream* stream)
{
    if (stream == &StdOut)
        return GetFileDescriptor(stdout);
    else if (stream == &StdErr)
        return GetFileDescriptor(stderr);
    return InvalidHandle;
}

#if _MSC_VER > 1900 // Versions after VS 2015
#pragma warning(disable: 5039)
#endif
#include <windows.h>
#if _MSC_VER > 1900 // Versions after VS 2015
#pragma warning(default: 5039)
#endif
#include <processenv.h>

static std::map<ConsoleColor, int> s_colorLookup {
    { ConsoleColor::Black, 0 },
    { ConsoleColor::Red, FOREGROUND_RED },
    { ConsoleColor::Green, FOREGROUND_GREEN },
    { ConsoleColor::Yellow, FOREGROUND_RED | FOREGROUND_GREEN },
    { ConsoleColor::Blue, FOREGROUND_BLUE },
    { ConsoleColor::Magenta, FOREGROUND_RED | FOREGROUND_BLUE },
    { ConsoleColor::Cyan, FOREGROUND_GREEN | FOREGROUND_BLUE },
    { ConsoleColor::White, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE },
};

static std::uint16_t GetColorCode(ConsoleColor color)
{
    if (s_colorLookup.find(color & ConsoleColor::ColorMask) != s_colorLookup.end())
        return static_cast<std::uint16_t>(s_colorLookup[color & ConsoleColor::ColorMask]);
    return 0;
}

Console::Console(int handle)
    : m_handle(handle)
    , m_stream(DetermineStream(m_handle))
    , m_forceUseColor()
    , m_currentForegroundColor(ConsoleColor::Default)
    , m_currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(FILE* file)
    : m_handle(GetFileDescriptor(file))
    , m_stream(DetermineStream(m_handle))
    , m_forceUseColor()
    , m_currentForegroundColor(ConsoleColor::Default)
    , m_currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(std::ostream& stream)
    : m_handle(DetermineHandle(&stream))
    , m_stream(&stream)
    , m_forceUseColor()
    , m_currentForegroundColor(ConsoleColor::Default)
    , m_currentBackgroundColor(ConsoleColor::Default)
{
}

void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    m_stream->flush();

    if (m_handle != -1)
    {
        std::uint16_t colorAttributes = GetColorCode(ConsoleColor::White);
        if (foregroundColor != ConsoleColor::Default)
        {
            colorAttributes = static_cast<std::uint16_t>((colorAttributes & ~static_cast<std::uint16_t>(ConsoleColor::ColorMask)) | GetColorCode(foregroundColor));
            if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
                colorAttributes |= FOREGROUND_INTENSITY;
        }
        if (backgroundColor != ConsoleColor::Default)
        {
            colorAttributes = static_cast<std::uint16_t>((colorAttributes & ~(static_cast<std::uint16_t>(ConsoleColor::ColorMask)) << 4) | (GetColorCode(backgroundColor) << 4));
            if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
                colorAttributes |= BACKGROUND_INTENSITY;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorAttributes);
    }

    m_currentForegroundColor = foregroundColor;
    m_currentBackgroundColor = backgroundColor;
}

bool Console::IsTTY() const
{
    return ::IsTTY(m_handle);
}

bool Console::ShouldUseColor() const
{
    if (ForceUseColor())
        return true;
    if (m_handle == InvalidHandle)
        return false;
    if (!IsTTY())
        return false;

    return true;
}

bool Console::ForceUseColor() const
{
    return m_forceUseColor;
}

void Console::ForceUseColor(bool value)
{
    m_forceUseColor = value;
}

} // namespace utility

utility::SetConsoleForegroundColor fgcolor(utility::ConsoleColor color)
{
    return {color};
}

utility::SetConsoleBackgroundColor bgcolor(utility::ConsoleColor color)
{
    return {color};
}
