#include "test-platform/GoogleTest.h"

#include "utility/Console.h"

namespace utility {

TEST(ConsoleTest, ConstructorDefault)
{
    Console console;
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdOut, console.GetStream());
}

TEST(ConsoleTest, ConstructorHandleStdOut)
{
    Console console(STDOUT_FILENO);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdOut, console.GetStream());
}

TEST(ConsoleTest, ConstructorHandleStdErr)
{
    Console console(STDERR_FILENO);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdErr, console.GetStream());
}

TEST(ConsoleTest, ConstructorCustomStream)
{
    std::ostringstream stream;
    Console console(stream);
    EXPECT_FALSE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&stream, console.GetStream());
}

TEST(ConsoleTest, ConstructorStdOut)
{
    Console console(stdout);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdOut, console.GetStream());
}

TEST(ConsoleTest, ConstructorStdErr)
{
    Console console(stderr);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdErr, console.GetStream());
}

TEST(ConsoleTest, ConstructorCOut)
{
    Console console(StdOut);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdOut, console.GetStream());
}

TEST(ConsoleTest, ConstructorCErr)
{
    Console console(StdErr);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
    EXPECT_EQ(&StdErr, console.GetStream());
}

TEST(ConsoleTest, ForceUseColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.ForceUseColor(true);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_TRUE(console.ForceUseColor());
}

TEST(ConsoleTest, OutputNoColor)
{
    std::ostringstream stream;
    Console console(stream);
    console << "no color\n"
            << fgcolor(ConsoleColor::Red) << "red\n"
            << fgcolor(ConsoleColor::Green | ConsoleColor::Intensity)
            << bgcolor(ConsoleColor::White | ConsoleColor::Intensity) << "green on black\n"
            << fgcolor(ConsoleColor::Default) << bgcolor(ConsoleColor::Default) << "no color\n";
    std::string expected("no color\nred\ngreen on black\nno color\n");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, OutputColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.ForceUseColor(true);
    console << "no color\n"
            << fgcolor(ConsoleColor::Red) << "red\n"
            << fgcolor(ConsoleColor::Green | ConsoleColor::Intensity)
            << bgcolor(ConsoleColor::White | ConsoleColor::Intensity) << "green on black\n"
            << fgcolor(ConsoleColor::Default) << bgcolor(ConsoleColor::Default) << "no color\n";
    std::string expected("no color\nred\ngreen on black\nno color\n");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetForegroundColorNoColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.SetForegroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetForegroundColor(ConsoleColor::Default);
    console << "text";
    std::string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetForegroundColorWithColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.ForceUseColor(true);
    console.SetForegroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetForegroundColor(ConsoleColor::Default);
    console << "text";
#if defined(PLATFORM_LINUX)
    std::string expected("\033[0;34;1mtext\033[0mtext");
#else
    std::string expected("texttext");
#endif
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetBackgroundColorNoColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.SetBackgroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetBackgroundColor(ConsoleColor::Default);
    console << "text";
    std::string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetBackgroundColorWithColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.ForceUseColor(true);
    console.SetBackgroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetBackgroundColor(ConsoleColor::Default);
    console << "text";
    std::string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetResetTerminalColorNoColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.SetTerminalColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.ResetTerminalColor();
    console << "text";
    std::string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST(ConsoleTest, SetResetTerminalColorWithColor)
{
    std::ostringstream stream;
    Console console(stream);
    console.ForceUseColor(true);
    console.SetTerminalColor(ConsoleColor::Blue | ConsoleColor::Bold, ConsoleColor::Red | ConsoleColor::Intensity);
    console << "text";
    console.ResetTerminalColor();
    console << "text";
    std::string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

} // namespace utility
