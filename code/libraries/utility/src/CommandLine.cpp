#include "utility/CommandLine.h"

#include <cstdlib>
#include <iostream>
#include <windows.h>

#define BUFSIZE 4096 

namespace utility {

static HANDLE g_childStdInReadHandle = nullptr;
static HANDLE g_childStdInWriteHandle = nullptr;
static HANDLE g_childStdOutReadHandle = nullptr;
static HANDLE g_childStdOutWriteHandle = nullptr;
static HANDLE g_childStdErrReadHandle = nullptr;
static HANDLE g_childStdErrWriteHandle = nullptr;

static DWORD CreateChildProcess(const std::string& command);
static void WriteToPipeStdIn(const std::string& stdinText);
static void ReadFromPipeStdOut(std::string& stdoutText);
static void ReadFromPipeStdErr(std::string& stderrText);

static bool CloseHandle(HANDLE& handle)
{
    if (handle == nullptr)
        return true;
    bool result = ::CloseHandle(handle);
    handle = nullptr;
    return result;
}

int System(const std::string& commandLine)
{
    return system(commandLine.c_str());
}

int SystemRedirect(const std::string& commandLine, const std::string& stdinText, std::string& stdoutText, std::string& stderrText)
{
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited. 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = nullptr;

    // Create a pipe for the child process's STDOUT. 

    if (!::CreatePipe(&g_childStdOutReadHandle, &g_childStdOutWriteHandle, &saAttr, 0))
    {
        std::cerr << "StdOut CreatePipe" << std::endl;
        return 1;
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!::SetHandleInformation(g_childStdOutReadHandle, HANDLE_FLAG_INHERIT, 0))
    {
        std::cerr << "StdOut SetHandleInformation Read" << std::endl;
        return 1;
    }

    // Create a pipe for the child process's STDERR. 

    if (!::CreatePipe(&g_childStdErrReadHandle, &g_childStdErrWriteHandle, &saAttr, 0))
    {
        std::cerr << "StdErr CreatePipe" << std::endl;
        return 1;
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!::SetHandleInformation(g_childStdOutReadHandle, HANDLE_FLAG_INHERIT, 0))
    {
        std::cerr << "StdErr SetHandleInformation Read" << std::endl;
        return 1;
    }

    // Create a pipe for the child process's STDIN. 

    if (!::CreatePipe(&g_childStdInReadHandle, &g_childStdInWriteHandle, &saAttr, 0))
    {
        std::cerr << "StdIn CreatePipe" << std::endl;
        return 1;
    }

    // Ensure the write handle to the pipe for STDIN is not inherited. 

    if (!::SetHandleInformation(g_childStdInWriteHandle, HANDLE_FLAG_INHERIT, 0))
    {
        std::cerr << "StdIn SetHandleInformation Read" << std::endl;
        return 1;
    }

    // Create the child process. 

    auto result = CreateChildProcess(commandLine);

    // Write to the pipe that is the standard input for a child process. 
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.

    WriteToPipeStdIn(stdinText);

    ReadFromPipeStdOut(stdoutText);
    ReadFromPipeStdErr(stderrText);

    if (!CloseHandle(g_childStdInReadHandle))
    {
        std::cerr << "StdIn CloseHandle Read" << std::endl;
        return 1;
    }
    if (!CloseHandle(g_childStdInWriteHandle))
    {
        std::cerr << "StdIn CloseHandle Write" << std::endl;
        return 1;
    }
    if (!CloseHandle(g_childStdOutReadHandle))
    {
        std::cerr << "StdOut CloseHandle Read" << std::endl;
        return 1;
    }
    if (!CloseHandle(g_childStdOutWriteHandle))
    {
        std::cerr << "StdOut CloseHandle Write" << std::endl;
        return 1;
    }
    if (!CloseHandle(g_childStdErrReadHandle))
    {
        std::cerr << "StdErr CloseHandle Read" << std::endl;
        return 1;
    }
    if (!CloseHandle(g_childStdErrWriteHandle))
    {
        std::cerr << "StdErr CloseHandle Write" << std::endl;
        return 1;
    }

    return static_cast<int>(result);
}

DWORD CreateChildProcess(const std::string& commandLine)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    std::string cmdline = commandLine;
    PROCESS_INFORMATION procInfo;
    STARTUPINFOA startInfo;
    BOOL success = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&startInfo, sizeof(STARTUPINFOA));
    startInfo.cb = sizeof(STARTUPINFOA);
    startInfo.hStdError = g_childStdErrWriteHandle;
    startInfo.hStdOutput = g_childStdOutWriteHandle;
    startInfo.hStdInput = g_childStdInReadHandle;
    startInfo.dwFlags |= STARTF_USESTDHANDLES;
    DWORD exitCode{};

    // Create the child process. 

    success = CreateProcessA(nullptr,
        cmdline.data(), // command line 
        nullptr,        // process security attributes 
        nullptr,        // primary thread security attributes 
        TRUE,           // handles are inherited 
        0,              // creation flags 
        nullptr,        // use parent's environment 
        nullptr,        // use parent's current directory 
        &startInfo,     // STARTUPINFO pointer 
        &procInfo);     // receives PROCESS_INFORMATION 

     // If an error occurs, exit the application. 
    if (!success)
    {
        std::cerr << "CreateProcess" << std::endl;
        return 1;
    }
    else
    {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 

        CloseHandle(procInfo.hThread);

        // Close handles to the stdin and stdout pipes no longer needed by the child process.
        // If they are not explicitly closed, there is no way to recognize that the child process has ended.

        if (!CloseHandle(g_childStdErrWriteHandle))
        {
            std::cerr << "StdOut CloseHandle Write" << std::endl;
            return 1;
        }

        if (!CloseHandle(g_childStdOutWriteHandle))
        {
            std::cerr << "StdOut CloseHandle Write" << std::endl;
            return 1;
        }

        if (!CloseHandle(g_childStdInReadHandle))
        {
            std::cerr << "StdIn CloseHandle Read" << std::endl;
            return 1;
        }

        do
        {
            WaitForSingleObject(procInfo.hProcess, 1000);
            exitCode = STILL_ACTIVE;
            if (FALSE == GetExitCodeProcess(procInfo.hProcess, &exitCode))
            {
                std::cerr << "GetExitCodeProcess() failure: " << GetLastError() << std::endl;
                break;
            }
            else if (STILL_ACTIVE == exitCode)
            {
                std::cout << "Still running\n";
            }
        }
        while (exitCode == STILL_ACTIVE);

        CloseHandle(procInfo.hProcess);
    }
    return exitCode;
}

void WriteToPipeStdIn(const std::string& stdinText)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
    if (!stdinText.empty())
    {
        DWORD bytesWritten;
        BOOL success = WriteFile(g_childStdInWriteHandle, stdinText.c_str(), static_cast<DWORD>(stdinText.length()), &bytesWritten, nullptr);
        if (!success)
        {
            std::cerr << "StdIn write error" << std::endl;
        }
    }

    // Close the pipe handle so the child process stops reading. 

    if (!CloseHandle(g_childStdInWriteHandle))
    {
        std::cerr << "StdIn CloseHandle Write" << std::endl;
    }
}

void ReadFromPipeStdOut(std::string& stdoutText)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
    DWORD bytesRead;
    CHAR buffer[BUFSIZE];
    BOOL success = FALSE;

    for (;;)
    {
        success = ReadFile(g_childStdOutReadHandle, buffer, BUFSIZE, &bytesRead, nullptr);
        if (!success || bytesRead == 0) break;

        stdoutText.append(buffer, bytesRead);
    }
}

void ReadFromPipeStdErr(std::string& stderrText)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
    DWORD bytesRead;
    CHAR buffer[BUFSIZE];
    BOOL success = FALSE;

    for (;;)
    {
        success = ReadFile(g_childStdOutReadHandle, buffer, BUFSIZE, &bytesRead, nullptr);
        if (!success || bytesRead == 0) break;

        stderrText.append(buffer, bytesRead);
    }
}

} // namespace utility