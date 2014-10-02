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

/** Extracting path string from file path. Ex "folder/xx/yy.png" returns "folder/xx/". */
string extractPath(const string& filePath);

/** Extracting extension from file path. Ex "folder/xx/yy.png" returns "folder/xx/yy". */
string extractExtension(const string& filePath);

/** Returns true, if whatPath path is wherePath path. */
bool isPathInsideOtherPath(const string& whatPath, const string& wherePath, bool strongly = false);

/** Format path string to normalized. Ex "asd/das/dasd". */
string normalizePathString(const string& path);

/** Returns character form virtual code. */
uint16 getUnicodeFromVirtualCode( uint8 code );

/** Converting int value to string. */
string toString(int value);

/** Converting float value to string. */
string toString(float value);

/** Converting color value to string. */
string toString(const color4& value);

/** Converting vector value to string. */
string toString(const vec2f& value);

/** Converting string to integer. */
int toInt(const string& str);

/** Converting string to float. */
float toFloat(const string& str);

CLOSE_O2_NAMESPACE

#endif //STRING_H