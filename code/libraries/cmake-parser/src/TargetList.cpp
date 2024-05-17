#include "cmake-parser/TargetList.h"

#include <sstream>

using namespace cmake_parser;

TargetList::TargetList()
    : m_targets{}
{
}

TargetPtr TargetList::FindTarget(const std::string& name) const
{
    auto it = m_targets.find(name);
    return (it == m_targets.end()) ? nullptr : it->second;
}

void TargetList::AddTarget(const std::string& name, TargetPtr target)
{
    m_targets.insert(std::pair(name, target));
}

std::string TargetList::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    switch (format)
    {
    case SerializationFormat::Text:
        stream << "TargetList:" << std::endl;
        for (auto const& target : m_targets)
        {
            if (target.second != nullptr)
                stream << target.second->Serialize() << std::endl;
        }
        break;
    case SerializationFormat::JSON:
        stream << SerializeList(format, indent, *this);
        break;
    default:
        break;
    }
    return stream.str();
}

