#include "EngineException.h"

#include <sstream>

EngineException::EngineException(int line, const char* file) :
	line(line),
	file(file)
{
}

const char* EngineException::what() const
{
	std::ostringstream oss;
	oss << GetType() << " occured in " << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* EngineException::GetType() const
{
	return "Unknown Engine Exception";
}

int EngineException::GetLine() const
{
	return line;
}

const std::string& EngineException::GetFile() const
{
	return file;
}

std::string EngineException::GetOriginString() const
{
	std::ostringstream oss;
	oss << file << " at line " << line;
	return oss.str();
}
