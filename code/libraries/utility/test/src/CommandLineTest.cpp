#include "test-platform/GoogleTest.h"

#include "utility/CommandLine.h"

namespace utility {

TEST(CommandLineTest, Echo)
{
	EXPECT_EQ(0, System("echo Hello"));
}

TEST(CommandLineTest, EchoRedirected)
{
	std::string stdOutText;
	std::string stdErrText;
	EXPECT_EQ(0, SystemRedirect("where cmake", "", stdOutText, stdErrText));
	EXPECT_EQ("C:\\Program Files\\CMake\\bin\\cmake.exe\r\n", stdOutText);
	EXPECT_EQ("", stdErrText);
}

} // namespace utility
