#ifndef TYPES_H
#define TYPES_H

#include <string>

typedef unsigned long long uint64;
typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;

typedef std::basic_string< uint16, std::char_traits<uint16>, std::allocator<uint16> > wstring;

#endif //TYPES_H