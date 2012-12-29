#include "string_utils.h"

#include <cstdarg>

std::string formatStr(const char* str, ...)
{
	va_list vlist;
	va_start(vlist, str);

	char buf[1024]; 
	vsprintf(buf, str, vlist);

	va_end(vlist);

	return buf;
}

vec2 StrToVec2(const char* str, char delimer)
{
	int len = strlen(str);
	vec2 ret;
	char buf[10];
	for (int i = 0, j = 0, k = 0; i < len + 1; i++)
	{
		if (i == len || str[i] == delimer)
		{
			ret[k++] = (float)atof(buf);
			if (k == 2) break;
			j = 0;
			continue;
		}
		if (!isNumber(str[i]) && str[i] != '.') continue;

		buf[j++] = str[i]; buf[j] = '\0';
	}

	return ret;
}

vec3 StrToVec3(const char* str, char delimer)
{
	int len = strlen(str);
	vec3 ret;
	char buf[10];
	for (int i = 0, j = 0, k = 0; i < len + 1; i++)
	{
		if (i == len || str[i] == delimer)
		{
			ret[k++] = (float)atof(buf);
			if (k == 3) break;
			j = 0;
			continue;
		}
		if (!isNumber(str[i]) && str[i] != '.') continue;

		buf[j++] = str[i]; buf[j] = '\0';
	}

	return ret;
}

int StrToInt(const char* str)
{
	return atoi(str);
}

float StrToFloat(const char* str)
{
	return (float)atof(str);
}

std::string toStr(float val)
{
	return formatStr("%f", val);
}

std::string toStr(int val)
{
	return formatStr("%i", val);
}