#include "cmake-parser/List.h"

#include <sstream>
#include "utility/StringFunctions.h"

using namespace cmake_parser;

List::List()
    : m_items{}
{
}
List::List(const std::string& contents)
    : m_items{}
{
	auto items = utility::Split(contents, ';');
	for (auto const& item : items)
	{
		if (!item.empty())
			m_items.push_back(item);
	}
}

void List::Prepend(const std::string& value)
{
	auto items = utility::Split(value, ';');
	std::vector<std::string> newItems;
	for (auto const& item : items)
	{
		if (!item.empty())
			newItems.push_back(item);
	}
	m_items.insert(m_items.begin(), newItems.begin(), newItems.end());
}

void List::Append(const std::string& value)
{
	auto items = utility::Split(value, ';');
	for (auto const& item : items)
	{
		if (!item.empty())
			m_items.push_back(item);
	}
}

std::string List::ToString() const
{
	std::string result;
	for (auto const& item : m_items)
	{
		if (!result.empty())
			result += ";";
		result += item;
	}
	return result;
}

std::string List::Serialize(SerializationFormat format, unsigned indent) const
{
    std::ostringstream stream;
    stream << std::string(indent, ' ');
    switch (format)
    {
    case SerializationFormat::Text:
        stream  << "List " << ToString();
        break;
    case SerializationFormat::JSON:
		{
			stream << "[";
			bool first = true;
			for (auto const& item : m_items)
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
				stream << std::string(indent + 4, ' ') << SerializeString(format, item);
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

