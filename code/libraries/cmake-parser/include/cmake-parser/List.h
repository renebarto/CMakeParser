#pragma once

#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include "cmake-parser/Serialization.h"

namespace cmake_parser {

struct List
{
    std::vector<std::string> m_items;

    List();
    List(const std::string& contents);
    const std::vector<std::string>& Items() const { return m_items; }
    bool Empty() const { return m_items.empty(); }
    size_t Count() const { return m_items.size(); }
    void Prepend(const std::string& value);
    void Append(const std::string& value);
    std::string ToString() const;

    std::string Serialize(SerializationFormat format = SerializationFormat::Text, unsigned indent = 0) const;
};
using ListPtr = std::shared_ptr<List>;

inline std::ostream& operator << (std::ostream& stream, const List& value)
{
    return stream << value.Serialize();
}

inline std::ostream& operator << (std::ostream& stream, const ListPtr& value)
{
    return stream << ((value == nullptr) ? "null" : value->Serialize());
}

} // namespace cmake_parser