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

/** Converting int value to string. */
std::string toString(int value);

/** Converting float value to string. */
std::string toString(float value);

/** Converting string to integer. */
int toInt(const std::string& str);

/** Converting string to float. */
float toFloat(const std::string& str);

CLOSE_O2_NAMESPACE

#endif //STRING_H