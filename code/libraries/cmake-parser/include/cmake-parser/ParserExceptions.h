#pragma once

#include <set>
#include "parser/Token.h"

namespace cmake_parser {

class UnexpectedToken
    : public std::exception
{
private:
    std::string m_message;
    parser::SourceLocation m_location;

public:
    UnexpectedToken(const parser::Token<Terminal>& token, const char* fileName, int line)
        : m_message{ "Unexpected token: " + token.Serialize() }
        , m_location{ fileName, line, 1 }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
    const parser::SourceLocation& Location() const { return m_location; }
};

class UnexpectedEndOfStream
    : public std::exception
{
private:
    std::string m_message;
    parser::SourceLocation m_location;

public:
    UnexpectedEndOfStream(const char* fileName, int line)
        : m_message{ "Unexpected end of stream" }
        , m_location{ fileName, line, 1 }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
    const parser::SourceLocation& Location() const { return m_location; }
};

class InvalidMessageMode
    : public std::exception
{
private:
    std::string m_message;
    parser::SourceLocation m_location;

public:
    InvalidMessageMode(const parser::Token<Terminal>& token, const char* fileName, int line)
        : m_message{ "Invalid message mode: " + token.Value() }
        , m_location{ fileName, line, 1 }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
    const parser::SourceLocation& Location() const { return m_location; }
};


} // namespace cmake_parser
