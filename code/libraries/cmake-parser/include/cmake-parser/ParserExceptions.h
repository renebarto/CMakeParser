#pragma once

#include <set>
#include "parser/Token.h"

namespace cmake_parser {

class UnexpectedToken
    : public std::exception
{
private:
    std::string m_message;

public:
    UnexpectedToken(const parser::Token<Terminal>& token)
        : m_message{ "Unexpected token: " + token.Serialize() }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
};

class UnexpectedEndOfStream
    : public std::exception
{
private:
    std::string m_message;

public:
    UnexpectedEndOfStream()
        : m_message{ "Unexpected end of stream" }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
};

class InvalidMessageMode
    : public std::exception
{
private:
    std::string m_message;

public:
    InvalidMessageMode(const parser::Token<Terminal>& token)
        : m_message{ "Invalid message mode: " + token.Value() }
    {}
    const char* what() const override
    {
        return m_message.c_str();
    }
};


} // namespace cmake_parser
