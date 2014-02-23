#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "public.h"
#include <vector>

OPEN_O2_NAMESPACE
	
#define VECTOR_CONTAINS(vectype, vec, pred) { \
		for (vectype::iterator it = vec.begin(); it != vec.end(); ++it) \
			if (pred) return true; \
		return false; \
	}

#define CVECTOR_CONTAINS(vectype, vec, pred) { \
		for (vectype::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) \
			if (pred) return true; \
		return false; \
	}


CLOSE_O2_NAMESPACE

#endif // DATA_UTILS_H
