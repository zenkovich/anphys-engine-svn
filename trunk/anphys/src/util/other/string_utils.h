#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

#include "../math/vec2.h"
#include "../math/vector.h"
#include "../types.h"

std::string formatStr(const char* str, ...);

int		StrToInt(const char* str);
float	StrToFloat(const char* str);
vec2	StrToVec2(const char* str, char delimer = ';');
vec3	StrToVec3(const char* str, char delimer = ';');

std::string toStr(float val);
std::string toStr(int val);

inline bool isNumber(char c) { return !(c < '0' || c > '9'); }

/** adjust string size, alig: 0 - left 1 - center 2 - right */
std::string adjustStrSize(const std::string& str, unsigned int size, short alig = 0, char space = ' ');

wstring convertStringToWide(const std::string& str);
std::string convertWideToString(const wstring& wide);
uint16 getUnicodeFromVirtualCode(uint8 code);

#endif //STRING_UTILS_H