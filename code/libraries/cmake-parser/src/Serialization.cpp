#include "cmake-parser/Serialization.h"

#include <sstream>
#include "utility/StringFunctions.h"
#include "cmake-parser/DirectoryList.h"
#include "cmake-parser/ProjectList.h"
#include "cmake-parser/TargetList.h"
#include "cmake-parser/TypedVariableList.h"
#include "cmake-parser/VariableList.h"

namespace cmake_parser {

std::string SerializeString(SerializationFormat format, const std::string& str)
{
	std::ostringstream stream;
	switch (format)
	{
	case SerializationFormat::Text:
		stream << str;
		break;
	case SerializationFormat::JSON:
		{
			std::string result = utility::Replace(str, "\\", "\\\\");
			if (result.empty())
				return "null";
			stream << "\"" << result << "\"";
		}
		break;
	default:
		break;
	}
	return stream.str();
}

std::string SerializeList(SerializationFormat format, unsigned indent, const TypedVariableList& variables)
{
	std::ostringstream stream;
	stream << std::string(indent, ' ') << "[";
	bool first = true;
	for (auto const& dir : variables.GetVariables())
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
		if (dir.second != nullptr)
			stream << dir.second->Serialize(format, indent + 4);
		else
			stream << "null";
	}
	if (!first)
	{
		stream << std::endl << std::string(indent, ' ');
	}
	stream << "]";
	return stream.str();
}

std::string SerializeList(SerializationFormat format, unsigned indent, const VariableList& variables)
{
	std::ostringstream stream;
	stream << std::string(indent, ' ') << "[";
	bool first = true;
	for (auto const& dir : variables.GetVariables())
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
		if (dir.second != nullptr)
			stream << dir.second->Serialize(format, indent + 4);
		else
			stream << "null";
	}
	if (!first)
	{
		stream << std::endl << std::string(indent, ' ');
	}
	stream << "]";
	return stream.str();
}

std::string SerializeList(SerializationFormat format, unsigned indent, const DirectoryList& directories)
{
	std::ostringstream stream;
	stream << std::string(indent, ' ') << "[";
	bool first = true;
	for (auto const& dir : directories.GetDirectories())
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
		if (dir.second != nullptr)
			stream << dir.second->Serialize(format, indent + 4);
		else
			stream << "null";
	}
	if (!first)
	{
		stream << std::endl << std::string(indent, ' ');
	}
	stream << "]";
	return stream.str();
}

std::string SerializeList(SerializationFormat format, unsigned indent, const ProjectList& projects)
{
	std::ostringstream stream;
	stream << std::string(indent, ' ') << "[";
	bool first = true;
	for (auto const& proj : projects.GetProjects())
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
		if (proj.second != nullptr)
			stream << proj.second->Serialize(format, indent + 4);
		else
			stream << "null";
	}
	if (!first)
	{
		stream << std::endl << std::string(indent, ' ');
	}
	stream << "]";
	return stream.str();
}

std::string SerializeList(SerializationFormat format, unsigned indent, const TargetList& targets)
{
	std::ostringstream stream;
	stream << std::string(indent, ' ') << "[";
	bool first = true;
	for (auto const& target : targets.GetTargets())
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
	return stream.str();
}

} // cmake_parser
