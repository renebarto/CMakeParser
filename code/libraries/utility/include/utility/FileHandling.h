#pragma once

#include <cstdio>
#include <iostream>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

extern std::ostream& StdOut;
extern std::ostream& StdErr;

typedef int FileDescriptor;
constexpr FileDescriptor InvalidHandle = static_cast<FileDescriptor>(-1);

FileDescriptor GetFileDescriptor(FILE * file);
bool IsTTY(int fd);
