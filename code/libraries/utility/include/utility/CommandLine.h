#pragma once

#include <string>

namespace utility {

int System(const std::string& commandLine);
int SystemRedirect(const std::string& commandLine, const std::string& stdinText, std::string& stdoutText, std::string& stderrText);

} // namespace utility