#pragma once

#include "cmake-parser/CMakeModel.h"
#include "cmake-parser/Lexer.h"
#include "parser/ParserExecutor.h"

namespace cmake_parser {

class CMakeParser
{
private:
    CMakeModel m_model;
    const std::filesystem::path m_rootDirectory;
    const std::string m_buildDirectoryName;
    std::istream& m_stream;

public:
    CMakeParser(const std::filesystem::path& rootDirectory, const std::string& buildDirectoryName, std::istream& stream);
    bool Parse();

    const CMakeModel& GetModel() const { return m_model; }
    CMakeModel& GetModel() { return m_model; }
    void Setup();
};

} // namespace cmake_parser
