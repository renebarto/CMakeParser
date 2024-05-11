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
        {
            stream << std::string(indent, ' ') << "[";
            bool first = true;
            for (auto const& target : m_targets)
            {
                if (first)
                {
                    stream << std::endl;
                }
                else
                {
                    stream << "," << std::endl;
                }
                first = false;
                if (target.second != nullptr)
                    stream << target.second->Serialize(format, indent + 4);
                else
                    stream << "null";
            }
            if (!first)
            {
                stream << std::endl << std::string(indent, ' ');
            }
            stream << "]";
    }
        break;
    default:
        break;
    }
    return stream.str();
}

