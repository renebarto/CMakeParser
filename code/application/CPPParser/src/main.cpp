#include "getopt.h"
#include <iostream>
//#include "parser/Lexer.h"

int main(int argc, char* argv[])
{
    int opt{};
    while ((opt = getopt(argc, argv, ":a:bc")) != -1)
    {
        switch (static_cast<char>(opt))
        {
        case 'a':
            std::cout << "Option with argument: " << static_cast<char>(opt) << ": " << optarg << std::endl;
            break;
        case 'b':
        case 'c':
            std::cout << "Option: " << static_cast<char>(opt) << std::endl;
            break;
        case '?':
        default:
            std::cout << "Invalid option: " << static_cast<char>(opt) << std::endl;
            break;
        }
    }
    for (; optind < argc; optind++) {
        std::cout << "Extra argument: " << argv[optind] << std::endl;
//        parser::ParseFile(argv[optind]);
    }
}
