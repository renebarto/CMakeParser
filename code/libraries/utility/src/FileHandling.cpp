#include "utility/FileHandling.h"

#include <cstdlib>

#if _MSC_VER > 1900 // Versions after VS 2015
#pragma warning(disable: 5039)
#endif
#include <windows.h>
#if _MSC_VER > 1900 // Versions after VS 2015
#pragma warning(default: 5039)
#endif
#include <io.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

std::ostream& StdOut = std::cout;
std::ostream& StdErr = std::cerr;

FileDescriptor GetFileDescriptor(FILE * file)
{
    return ::_fileno(file);
}
bool IsTTY(int fd)
{
    return ::_isatty(fd) != 0;
}
