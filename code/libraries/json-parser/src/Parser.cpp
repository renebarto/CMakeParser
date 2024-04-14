#include "json-parser/Parser.h"

#include "utility/StringFunctions.h"
#include "json-parser/json.h"
#include "json-parser/JSONFactory.h"

using namespace parser;
using namespace json_parser;

Parser::Parser(const std::string& path, std::istream& stream)
    : m_lexer(path, stream)
    , m_value{}
    , m_errorCount{}
{
}

bool Parser::Parse()
{
    m_value = ParseValue();
    return m_lexer.IsAtEnd() && (m_errorCount == 0);
}

JSONValuePtr Parser::GetValue()
{
    return m_value;
}

const JSONValuePtr Parser::GetValue() const
{
    return m_value;
}

TokenTypes Parser::Type(parser::Token& token)
{
    return static_cast<TokenTypes>(token.Type().m_type);
}

JSONValuePtr Parser::ParseValue()
{
    while (!m_lexer.IsAtEnd())
    {
        auto token = m_lexer.GetToken();
        if (token.IsNull())
        {
            OnNoMoreToken(m_lexer.GetCurrentLocation());
        }
        if (token.IsInvalid() || (token.IsNull() && !m_lexer.IsAtEnd()))
        {
            return nullptr;
            break;
        }
        if (!token.IsNull())
        {
            switch (Type(token))
            {
            case TokenTypes::Null:
                return CreateValue(nullptr);
            case TokenTypes::False:
                return CreateValue(false);
            case TokenTypes::True:
                return CreateValue(true);
            case TokenTypes::Number:
                {
                    auto value = token.Value();
                    token = m_lexer.GetToken();
                    if (Type(token) == TokenTypes::NumberExponent)
                    {
                        value += token.Value();
                    }
                    else
                    {
                        if (!token.IsNull())
                            m_lexer.UngetToken(token);
                    }
                    return std::make_shared<JSONNumber>(value);
                }
            case TokenTypes::String:
                return std::make_shared<JSONString>(token.Value());
            case TokenTypes::SquareBracketOpen:
                m_lexer.UngetToken(token);
                return ParseArray();
            case TokenTypes::CurlyBracketOpen:
                m_lexer.UngetToken(token);
                return ParseObject();
            default:
                return nullptr;
            }
        }
    }
    return nullptr;
}

JSONValuePtr Parser::ParseArray()
{
    auto result = CreateArray();
    Token token{};
    if (!Expect(TokenTypes::SquareBracketOpen, token))
        return nullptr;
    while (Expect({ TokenTypes::SquareBracketClose, TokenTypes::Null, TokenTypes::False, TokenTypes::True, TokenTypes::Number, TokenTypes::String, TokenTypes::SquareBracketOpen, TokenTypes::CurlyBracketOpen }, token))
    {
        if (Type(token) != TokenTypes::SquareBracketClose)
        {
            m_lexer.UngetToken(token);
            auto value = ParseValue();
            if (value == nullptr)
                break;
            result->AsArray()->AddValue(value);
            if (!Expect({ TokenTypes::SquareBracketClose, TokenTypes::Comma }, token))
                return result;
        }
        if (Type(token) == TokenTypes::SquareBracketClose)
        {
            return result;
        }
    }
    return nullptr;
}

JSONValuePtr Parser::ParseObject()
{
    auto result = CreateObject();
    Token token{};
    if (!Expect(TokenTypes::CurlyBracketOpen, token))
        return nullptr;
    while (Expect({ TokenTypes::CurlyBracketClose, TokenTypes::String }, token))
    {
        if (Type(token) != TokenTypes::CurlyBracketClose)
        {
            auto key = utility::UnQuote(token.Value());
            if (!Expect(TokenTypes::Colon, token))
                break;
            if (!Expect({ TokenTypes::Null, TokenTypes::False, TokenTypes::True, TokenTypes::Number, TokenTypes::String, TokenTypes::SquareBracketOpen, TokenTypes::CurlyBracketOpen }, token))
                break;
            m_lexer.UngetToken(token);
            auto value = ParseValue();
            if (value == nullptr)
                break;
            JSONKVPair kvPair{ key, value };
            result->AsObject()->AddPair(kvPair);
            if (!Expect({ TokenTypes::CurlyBracketClose, TokenTypes::Comma }, token))
                return result;
        }
        if (Type(token) == TokenTypes::CurlyBracketClose)
        {
            return result;
        }
    }
    return nullptr;
}

bool Parser::Expect(TokenTypes type, Token& token)
{
    do
    {
        token = m_lexer.GetToken();
    } while (Type(token) == TokenTypes::Whitespace);
    if (Type(token) != type)
    {
        OnParseError(token.Value(), token.BeginLocation(), token.EndLocation());
        return false;
    }
    return true;
}

bool Parser::Expect(std::set<TokenTypes> oneOfTypes, Token& token)
{
    do
    {
        token = m_lexer.GetToken();
    } while (Type(token) == TokenTypes::Whitespace);
    if (oneOfTypes.find(Type(token)) == oneOfTypes.end())
    {
        OnParseError(token.Value(), token.BeginLocation(), token.EndLocation());
        return false;
    }
    return true;
}

void Parser::OnNoMoreToken(const SourceLocation& /*location*/)
{
    std::cout << "No more tokens" << std::endl;
}

void Parser::OnParseError(const std::string& text, const SourceLocation& startLocation, const SourceLocation& endLocation)
{
    m_errorCount++;
    std::cout << "Parse error: unexpected token " << text << " at location " << startLocation << "-" << endLocation << std::endl;
}
