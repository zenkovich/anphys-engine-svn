#ifndef STRING_H
#define STRING_H

#include "public.h"

OPEN_O2_NAMESPACE
	
typedef std::basic_string<uint16> wstring;

/** Returns formatted string, Works same as sprintf(). */
std::string format(const char* str, ...);

/** Returns converted string to wide string. */
wstring convString2Wide(const std::string& str);

/** Returns converted wide string to string. */
std::string convWide2String(const wstring& wide);

CLOSE_O2_NAMESPACE

#endif //STRING_H