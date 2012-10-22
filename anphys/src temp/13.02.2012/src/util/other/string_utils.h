#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

std::string formatStr(std::string formatStr, ...);

int StrToInt(const std::string& str);
float StrToFloat(const std::string& str);

std::string toStr(float val);
std::string toStr(int val);

#endif //STRING_UTILS_H