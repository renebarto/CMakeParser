#pragma once

#include <vector>
#include "json-parser/JSONValue.h"

namespace json_parser {

using JSONValueList = std::vector<JSONValuePtr>;

class JSONArray
    : public JSONValue
{
public:
    JSONArray();
    JSONArray(const std::initializer_list<JSONValuePtr> & elements);
    virtual ~JSONArray() {};

    class Iterator
    {
    public:
        Iterator(JSONValueList& container)
            : m_begin(container.begin())
            , m_current(m_begin)
            , m_end(container.end())
        {
        }
        bool AtBegin() const { return m_current == m_begin; }
        bool AtEnd() const { return m_current == m_end; }
        void Reset() { m_current = m_begin; }
        JSONValueList::iterator begin() { return m_begin; }
        JSONValueList::iterator end() { return m_end; }
        JSONValuePtr operator++()
        {
            if (!AtEnd())
                ++m_current;
            return operator *();
        }
        JSONValuePtr operator++(int)
        {
            JSONValuePtr result = operator *();
            if (!AtEnd())
                ++m_current;
            return result;
        }

        JSONValuePtr operator *()
        {
            if (AtEnd())
                std::cout << "Iterator outside range" << std::endl;

            return *m_current;
        }

    private:
        JSONValueList::iterator m_begin;
        JSONValueList::iterator m_current;
        JSONValueList::iterator m_end;
    };

    class ConstIterator
    {
    public:
        ConstIterator(const JSONValueList & container)
        : m_begin(container.begin())
        , m_current(m_begin)
        , m_end(container.end())
        {
        }
        bool AtBegin() const { return m_current == m_begin; }
        bool AtEnd() const { return m_current == m_end; }
        void Reset() { m_current = m_begin; }
        JSONValueList::const_iterator begin() { return m_begin; }
        JSONValueList::const_iterator end() { return m_end; }
        JSONValuePtr operator++()
        {
            if (!AtEnd())
                ++m_current;
            return operator *();
        }
        JSONValuePtr operator++(int)
        {
            JSONValuePtr result = operator *();
            if (!AtEnd())
                ++m_current;
            return result;
        }

        JSONValuePtr operator *()
        {
            if (AtEnd())
                std::cout << "Iterator outside range" << std::endl;

            return *m_current;
        }

    private:
        JSONValueList::const_iterator m_begin;
        JSONValueList::const_iterator m_current;
        JSONValueList::const_iterator m_end;
    };

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual JSONValueType Type() const override { return JSONValueType::Array; }

    void Clear() { m_elements.clear(); }
    void AddValue(JSONValuePtr value);
    size_t Size() const { return m_elements.size(); }
    Iterator GetIterator() { return Iterator(m_elements); }
    ConstIterator GetIterator() const { return ConstIterator(m_elements); }

private:
    JSONValueList m_elements;
};

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONArray& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
