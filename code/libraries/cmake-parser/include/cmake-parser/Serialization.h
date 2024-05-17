#pragma once

#include <string>

namespace cmake_parser {

class DirectoryList;
class ProjectList;
class TargetList;
class TypedVariableList;
class VariableList;

enum class SerializationFormat
{
	Text,
	JSON,
};

std::string SerializeString(SerializationFormat format, const std::string& str);
std::string SerializeList(SerializationFormat format, unsigned indent, const TypedVariableList& variables);
std::string SerializeList(SerializationFormat format, unsigned indent, const VariableList& variables);
std::string SerializeList(SerializationFormat format, unsigned indent, const DirectoryList& directories);
std::string SerializeList(SerializationFormat format, unsigned indent, const ProjectList& projects);
std::string SerializeList(SerializationFormat format, unsigned indent, const TargetList& targets);

} // cmake_parser
