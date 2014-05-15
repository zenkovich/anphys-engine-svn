#include "string.h"

#include <cstdarg>

OPEN_O2_NAMESPACE

string format( const char* str, ... )
{
	const uint32 bufsz = 1096;
	static char buf[bufsz];

	va_list ap;
	va_start(ap, str);
	vsnprintf(buf, bufsz, str, ap);
	va_end(ap);

	return string(buf);
}

#ifdef PLATFORM_WIN
#include <Windows.h>

wstring convString2Wide(const string& str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), 0, 0);
	
	wstring retvalue;
	retvalue.resize(len, 0);
	wchar_t* retvalueptr = reinterpret_cast<wchar_t*>(const_cast<uint16*>(retvalue.data()));

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), retvalueptr, len);
	
	return retvalue;
}

string convWide2String(const wstring& wide)
{
	const wchar_t* wideptr = reinterpret_cast<const wchar_t*>(wide.data());

	int len =  WideCharToMultiByte(CP_ACP, 0, wideptr, wide.size(), NULL, 0, 0, 0);
	
	string retvalue;
	retvalue.resize(len, 0);
	char* retvalueptr = const_cast<char*>(retvalue.data());

	WideCharToMultiByte(CP_ACP, 0, wideptr, wide.size(), retvalueptr, len + 1, 0, 0);

	return retvalue;
}

string toString( int value )
{
	return format("%i", value);
}

string toString( float value )
{
	return format("%f", value);
}

int toInt( const string& str )
{
	return atoi(str.c_str());
}

float toFloat( const string& str )
{
	return (float)atof(str.c_str());
}

#endif //PLATFORM_WIN

CLOSE_O2_NAMESPACE