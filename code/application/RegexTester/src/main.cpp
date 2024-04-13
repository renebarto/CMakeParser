#include "getopt.h"
#include <iostream>
#include <regex>

int main(int argc, char* argv[])
{
    int opt{};
    while ((opt = getopt(argc, argv, ":a:bc")) != -1)
    {
        switch (static_cast<char>(opt))
        {
        case '?':
        default:
            std::cout << "Invalid option: " << static_cast<char>(opt) << std::endl;
            break;
        }
    }
    if ((optind + 2) > argc)
    {
        std::cout << "Pass two arguments: regex-tester regex string" << std::endl;
        return 1;
    }
    std::string regexArg = argv[optind];
    std::string stringArg = argv[optind + 1];
    std::cout << "Matching " << stringArg <<" with " << regexArg << std::endl;

    std::regex regex(regexArg);
    if (std::regex_match(stringArg, regex))
    {
        std::cout << "Match" << std::endl;
    }
    else
    {
        std::cout << "No match" << std::endl;
    }
}
