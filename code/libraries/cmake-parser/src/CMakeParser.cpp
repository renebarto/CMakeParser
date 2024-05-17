#include "cmake-parser/CMakeParser.h"

#include <fstream>
#include "utility/CommandLine.h"
#include "utility/StringFunctions.h"
#include "tracing/Tracing.h"
#include "cmake-parser/ScriptParser.h"

using namespace utility;
using namespace parser;
using namespace std::placeholders;

namespace cmake_parser {

static std::string FindCMake()
{
    std::string stdoutText;
    std::string stderrText;
    if (SystemRedirect("where cmake", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

static std::string GetCMakeVersion()
{
    std::string stdoutText;
    std::string stderrText;
    if (SystemRedirect("cmake --version", "", stdoutText, stderrText) == 0)
    {
        auto line = stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
        std::string prefix = "cmake version ";
        return line.substr(prefix.length());
    }
    return {};
}

static std::string FindNinja()
{
    std::string stdoutText;
    std::string stderrText;
    if (SystemRedirect("where ninja", "", stdoutText, stderrText) == 0)
        return stdoutText.substr(0, stdoutText.find_first_of("\r\n", 0));
    return {};
}

CMakeParser::CMakeParser(const std::filesystem::path& rootDirectory, const std::string& buildDirectoryName, std::istream& stream)
	: m_model{}
    , m_rootDirectory{ rootDirectory }
    , m_buildDirectoryName{ buildDirectoryName }
    , m_stream{ stream }
{
    Setup();
}

bool CMakeParser::Parse()
{
    ScriptParser parser{ m_model, m_rootDirectory, m_stream };
    return parser.Parse();
}

void CMakeParser::Setup()
{
    m_model.SetupSourceRoot(m_rootDirectory, m_buildDirectoryName);
    m_model.SetupCMakePath(FindCMake(), GetCMakeVersion());
    m_model.SetupNinjaPath(FindNinja());
}

std::string CMakeParser::Serialize() const
{
    return m_model.Serialize(SerializationFormat::JSON, 0);
}

} // namespace cmake_parser
