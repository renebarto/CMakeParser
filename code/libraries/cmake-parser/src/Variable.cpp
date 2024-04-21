#include "cmake-parser/Variable.h"

#include <sstream>

using namespace cmake_parser;

std::string Variable::Serialize() const
{
	std::ostringstream stream;
	stream << "Variable " << Name() << " = " << Value();
	return stream.str();
}

