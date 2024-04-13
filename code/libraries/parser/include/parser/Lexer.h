#pragma once

#include <string>
#include "SourceLocation.h"

namespace parser {

enum class SymbolType
{
    Identifier,
    Number,
};

struct Symbol
{
    SymbolType m_type;
    std::string m_path;
    SourceLocation m_location;
    std::string m_value;
};

struct Term
{
    std::string m_path;
    SourceLocation m_location;
    std::string m_value;

    Term() = default;
    Term(const std::string& path, const SourceLocation& location, const std::string& value)
        : m_path{ path }
        , m_location{ location }
        , m_value{ value }
    {
    }
    std::string Serialize() const
    {
        return m_path + ":" + m_location.Serialize() + ": " + m_value;
    }
    bool Equals(const Term& other) const
    {
        return ((m_path == other.m_path) && (m_location == other.m_location) && (m_value == other.m_value));
    }
};

inline bool operator ==(const Term& lhs, const Term& rhs)
{
    return lhs.Equals(rhs);
}
inline bool operator !=(const Term& lhs, const Term& rhs)
{
    return !(lhs == rhs);
}

class TermList
{
private:
    using Terms = std::vector<Term>;
    Terms m_terms;
    Terms::const_iterator m_it;

public:
    TermList()
        : m_terms{}
        , m_it{ m_terms.begin() }
    {}
    void AddTerm(const Term& term)
    {
        m_terms.push_back(term);
    }

    void Reset()
    {
        m_it = m_terms.begin();
    }
    bool NextTerm(Term& term)
    {
        if (!AtEnd())
        {
            term = *m_it;
            m_it++;
            return true;
        }
        return false;
    }
    bool AtEnd()
    {
        return m_it == m_terms.end();
    }
};

class Lexer
{
private:
    std::string m_path;
    std::istream& m_stream;
    TermList m_termList;

public:
    Lexer(const std::string& path, std::istream& stream);
    bool SplitIntoTerms();

    TermList& GetTerms();
};

bool ParseFile(const std::string& path);

bool Parse(const std::string& filePath, std::istream& stream);

} // namespace parser
