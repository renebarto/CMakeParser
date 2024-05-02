#include "cmake-parser/TypedVariable.h"

#include <sstream>

using namespace cmake_parser;

std::string TypedVariable::Serialize() const
{
	std::ostringstream stream;
	stream << "TypedVariable " << Name() << ":" << Type() << " = " << Value() << " (" << Description() << ")";
	return stream.str();
}

