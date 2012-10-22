#include "string_utils.h"

#include <cstdarg>

std::string formatStr(std::string formatStr, ...)
{
	va_list vlist;
	va_start(vlist, formatStr);

	char buf[1024];
	vsprintf(buf, formatStr.c_str(), vlist);

	va_end(vlist);

	return buf;
}

int StrToInt(const std::string& str)
{
	return atoi(str.c_str());
}

float StrToFloat(const std::string& str)
{
	return atof(str.c_str());
}

std::string toStr(float val)
{
	return formatStr("%f", val);
}

std::string toStr(int val)
{
	return formatStr("%i", val);
}