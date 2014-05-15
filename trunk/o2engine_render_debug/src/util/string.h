#ifndef STRING_H
#define STRING_H

#include "public.h"

OPEN_O2_NAMESPACE
	
typedef std::basic_string<uint16> wstring;

/** Returns formatted string, Works same as sprintf(). */
string format(const char* str, ...);

/** Returns converted string to wide string. */
wstring convString2Wide(const string& str);

/** Returns converted wide string to string. */
string convWide2String(const wstring& wide);

/** Converting int value to string. */
string toString(int value);

/** Converting float value to string. */
string toString(float value);

/** Converting string to integer. */
int toInt(const string& str);

/** Converting string to float. */
float toFloat(const string& str);

CLOSE_O2_NAMESPACE

#endif //STRING_H