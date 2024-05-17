#include "getopt.h"
#include <fstream>
#include <iostream>
#include "cmake-parser/CMakeParser.h"
#include "utility/Console.h"
#include "tracing/Tracing.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/TraceWriter.h"

using namespace utility;
using namespace tracing;
using namespace cmake_parser;

static Console console;

int ShowHelp(const char* application)
{
    console << fgcolor(ConsoleColor::Yellow) << "Usage: " << application << " <cmake file>" << fgcolor(ConsoleColor::Default) << std::endl;
    return 0;
}

void DisplayInfo(const CMakeModel& model)
{
    console << fgcolor(ConsoleColor::Cyan) << "Projects:" << fgcolor(ConsoleColor::Default) << std::endl;
    for (auto const& projectItem : model.GetProjects())
    {
        if (projectItem.second != nullptr)
        {
            auto project = projectItem.second;
            console << fgcolor(ConsoleColor::Cyan) << project->Name() << fgcolor(ConsoleColor::Default) << std::endl;
            if (project != model.GetMainProject())
            {
                console << fgcolor(ConsoleColor::Cyan) << "  Targets:" << fgcolor(ConsoleColor::Default) << std::endl;
                for (auto const& targetItem : project->GetTargets())
                {
                    auto target = targetItem.second;
                    console << fgcolor(ConsoleColor::Cyan) << "    " << target->Name() << fgcolor(ConsoleColor::Default) << std::endl;
                    console << fgcolor(ConsoleColor::Cyan) << "      Sources:" << fgcolor(ConsoleColor::Default) << std::endl;
                    auto const& buildEntries = target->GetBuildEntries();
                    for (auto const& buildEntry : buildEntries)
                    {
                        console << fgcolor(ConsoleColor::Cyan) << "        File:                    " << buildEntry.SourcePath().generic_string() << fgcolor(ConsoleColor::Default) << std::endl;
                        console << fgcolor(ConsoleColor::Cyan) << "        Definitions:             " << buildEntry.CompileDefinitions() << fgcolor(ConsoleColor::Default) << std::endl;
                        console << fgcolor(ConsoleColor::Cyan) << "        Definitions (interface): " << buildEntry.InterfaceCompileDefinitions() << fgcolor(ConsoleColor::Default) << std::endl;
                        console << fgcolor(ConsoleColor::Cyan) << "        Options:                 " << buildEntry.CompileOptions() << fgcolor(ConsoleColor::Default) << std::endl;
                        console << fgcolor(ConsoleColor::Cyan) << "        Options (interface):     " << buildEntry.InterfaceCompileOptions() << fgcolor(ConsoleColor::Default) << std::endl;
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    int opt{};
    while ((opt = getopt(argc, argv, ":a:bc")) != -1)
    {
        switch (static_cast<char>(opt))
        {
        //case 'a':
        //    std::cout << "Option with argument: " << static_cast<char>(opt) << ": " << optarg << std::endl;
        //    break;
        //case 'b':
        //case 'c':
        //    std::cout << "Option: " << static_cast<char>(opt) << std::endl;
        //    break;
        case '?':
            return ShowHelp(argv[0]);
        default:
            console << fgcolor(ConsoleColor::Red) << "Invalid option: " << static_cast<char>(opt) << fgcolor(ConsoleColor::Default) << std::endl;
            break;
        }
    }
    CategorySet<TraceCategory> savedTraceFilter;
    ConsoleTraceLineWriter lineWriter;
    TraceWriter traceWriter{ lineWriter };

    Tracing::SetTraceWriter(&traceWriter);
    savedTraceFilter = Tracing::GetDefaultTraceFilter();
    Tracing::SetDefaultTraceFilter(TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information | TraceCategory::Data | TraceCategory::Debug);
    for (; optind < argc; optind++)
    {
        std::filesystem::path rootPath = argv[optind];
        std::filesystem::path binaryPath = rootPath / "cmake-build";
        auto filePath = rootPath / "CMakeLists.txt";
        std::ifstream stream(filePath);
        CMakeParser parser(rootPath.generic_string(), binaryPath.generic_string(), stream);

        console << fgcolor(ConsoleColor::Cyan) << "Parsing file: " << argv[optind] << fgcolor(ConsoleColor::Default) << std::endl;
        if (!parser.Parse())
        {
            console << fgcolor(ConsoleColor::Red) << "Parsing failed: " << argv[optind] << fgcolor(ConsoleColor::Default) << std::endl;
            return 1;
        }
        DisplayInfo(parser.GetModel());
    }
    Tracing::SetTraceWriter(nullptr);
    Tracing::SetDefaultTraceFilter(savedTraceFilter);
    return 0;
}
