#ifndef PUBLIC_H
#define PUBLIC_H

//namespace macros
#define OPEN_O2_NAMESPACE namespace o2 {
#define CLOSE_O2_NAMESPACE }

//safe release objects
#define safe_release(obj) { if (obj != 0) delete obj; }
#define safe_release_arr(obj) { if (obj != 0) delete[] obj; }

//some includes
#include "util/assert.h"
#include <string>

//types
typedef unsigned long long uint64;
typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef long long          int64;

#endif //PUBLIC_H