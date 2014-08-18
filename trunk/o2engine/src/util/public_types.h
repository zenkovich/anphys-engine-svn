#ifndef PUBLIC_TYPES_H
#define PUBLIC_TYPES_H

#include <string>
#include <vector>

typedef unsigned long long uint64;
typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef long long          int64;


OPEN_O2_NAMESPACE

//some basic types
#define vector std::vector
typedef std::string string;

#define StringsVec vector<string>

CLOSE_O2_NAMESPACE

#endif //PUBLIC_TYPES_H