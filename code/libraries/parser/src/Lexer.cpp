#include "parser/Lexer.h"

#include <fstream>
#include <vector>
#include "parser/Reader.h"

using namespace parser;

Lexer::Lexer(const std::string& path, std::istream& stream)
	: m_path{ path }
	, m_stream { stream }
	, m_termList{}
{
}

bool Lexer::SplitIntoTerms()
{
	Reader reader(m_path, m_stream);
	char ch;
	std::string str{};
	SourceLocation currentLocation = reader.GetLocation();
	SourceLocation previousLocation;
	SourceLocation termLocation;
	while (reader.GetChar(ch))
	{
		previousLocation = currentLocation;
		currentLocation = reader.GetLocation();
		if (std::isspace(ch))
		{
			if (!str.empty())
				m_termList.AddTerm(Term{ m_path, termLocation, str });
			str = {};
			continue;
		}
		if (str.empty())
			termLocation = previousLocation;
		str += ch;
	}
	if (!str.empty())
		m_termList.AddTerm(Term{ m_path, termLocation, str });
	m_termList.Reset();
	return true;
}

TermList& Lexer::GetTerms()
{
	return m_termList;
}

bool parser::ParseFile(const std::string& path)
{
	std::ifstream stream(path);
	if (!stream.good())
		return false;
	return Parse(path, stream);
}

bool parser::Parse(const std::string& filePath, std::istream& stream)
{
	Lexer lexer(filePath, stream);
	return lexer.SplitIntoTerms();
}
