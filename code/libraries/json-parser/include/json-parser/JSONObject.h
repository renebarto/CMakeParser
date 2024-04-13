#pragma once

#include <map>
#include <vector>
#include "json-parser/JSONValue.h"

namespace json_parser {

class JSONKVPair
{
public:
    JSONKVPair();
    JSONKVPair(const std::string& key, const JSONValuePtr& value);

    const std::string& GetKey() const { return m_key; }
    void SetKey(const std::string& value) { m_key = value; }
    const JSONValuePtr GetValue() const { return m_value; }
    void SetValue(const JSONValuePtr value) { m_value = value; }
    void Serialize(std::ostream& stream, int indentDepth = 0) const;

private:
    std::string m_key;
    JSONValuePtr m_value;
};

using JSONKVList = std::map<std::string, JSONValuePtr>;

class JSONObject : public JSONValue
{
public:
    JSONObject();
    JSONObject(const std::initializer_list<JSONKVPair>& elements);
    virtual ~JSONObject() {};

    class Iterator
    {
    public:
        Iterator(JSONKVList& container)
            : m_begin(container.begin())
            , m_current(m_begin)
            , m_end(container.end())
        {
        }
        bool AtBegin() const { return m_current == m_begin; }
        bool AtEnd() const { return m_current == m_end; }
        void Reset() { m_current = m_begin; }
        JSONKVList::iterator begin() { return m_begin; }
        JSONKVList::iterator end() { return m_end; }
        JSONKVPair operator++()
        {
            if (!AtEnd())
                ++m_current;
            return operator *();
        }
        JSONKVPair operator++(int)
        {
            JSONKVPair result = operator *();
            if (!AtEnd())
                ++m_current;
            return result;
        }

        JSONKVPair operator *()
        {
            if (AtEnd())
                std::cout << "Iterator outside range" << std::endl;

            return JSONKVPair(m_current->first, m_current->second);
        }

    private:
        JSONKVList::iterator m_begin;
        JSONKVList::iterator m_current;
        JSONKVList::iterator m_end;
    };

    class ConstIterator
    {
    public:
        ConstIterator(const JSONKVList& container)
            : m_begin(container.begin())
            , m_current(m_begin)
            , m_end(container.end())
        {
        }
        bool AtBegin() const { return m_current == m_begin; }
        bool AtEnd() const { return m_current == m_end; }
        void Reset() { m_current = m_begin; }
        JSONKVList::const_iterator begin() { return m_begin; }
        JSONKVList::const_iterator end() { return m_end; }
        JSONKVPair operator++()
        {
            if (!AtEnd())
                ++m_current;
            return operator *();
        }
        JSONKVPair operator++(int)
        {
            JSONKVPair result = operator *();
            if (!AtEnd())
                ++m_current;
            return result;
        }

        JSONKVPair operator *()
        {
            if (AtEnd())
                std::cout << "Iterator outside range" << std::endl;

            return JSONKVPair(m_current->first, m_current->second);
        }

    private:
        JSONKVList::const_iterator m_begin;
        JSONKVList::const_iterator m_current;
        JSONKVList::const_iterator m_end;
    };

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    void Clear() { m_elements.clear(); }
    JSONValueType Type() const override { return JSONValueType::Object; }
    bool AddPair(const JSONKVPair& pair);
    bool AddPair(const std::string& key, JSONValuePtr value)
    {
        return AddPair(JSONKVPair(key, value));
    }
    JSONValuePtr Find(const std::string& name);
    const JSONValuePtr Find(const std::string& name) const;
    const JSONBooleanPtr GetBoolean(const std::string& name) const;
    const JSONNumberPtr GetNumber(const std::string& name) const;
    const JSONStringPtr GetString(const std::string& name) const;
    const JSONArrayPtr GetArray(const std::string& name) const;
    const JSONObjectPtr GetObject(const std::string& name) const;

    size_t Size() const { return m_elements.size(); }
    Iterator GetIterator() { return Iterator(m_elements); }
    ConstIterator GetIterator() const { return ConstIterator(m_elements); }

private:
    JSONKVList m_elements;
};

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONObject& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
