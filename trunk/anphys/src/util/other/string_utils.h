#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

#include "../math/vec2.h"
#include "../math/vector.h"

std::string formatStr(const char* str, ...);

int		StrToInt(const char* str);
float	StrToFloat(const char* str);
vec2	StrToVec2(const char* str, char delimer = ';');
vec3	StrToVec3(const char* str, char delimer = ';');

std::string toStr(float val);
std::string toStr(int val);

inline bool isNumber(char c) { return !(c < '0' || c > '9'); }

#endif //STRING_UTILS_H