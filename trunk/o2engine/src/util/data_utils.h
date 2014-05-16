#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "public.h"

OPEN_O2_NAMESPACE
	
#define VECTOR_CONTAINS(vectype, vec, pred) { \
		for (vectype::iterator it = vec.begin(); it != vec.end(); ++it) \
			if (pred) return true; \
		return false; \
	}

#define VECTOR_CONTAINS_CONST(vectype, vec, pred) { \
		for (vectype::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) \
			if (pred) return true; \
		return false; \
	}
	
#define FIND(_vector, _search_object) \
	std::find((_vector).begin(), (_vector).end(), (_search_object))
	
#define FINDS_CONST(_vector, _search_object) \
	std::find((_vector).cbegin(), (_vector).cend(), (_search_object))

#define FOREACH(_vector_type, _vector, _iterator) \
	for (_vector_type::iterator _iterator = (_vector).begin(); _iterator != (_vector).end(); ++_iterator)

#define FOREACH_CONST(_vector_type, _vector, _iterator) \
	for (_vector_type::const_iterator _iterator = (_vector).cbegin(); _iterator != (_vector).cend(); ++_iterator)

#define FOREACH_BACK(_vector_type, _vector, _iterator) \
	for (_vector_type::iterator _iterator = (_vector).end(); _iterator != (_vector).begin(); --_iterator)

#define FOREACH_BACK_CONST(_vector_type, _vector, _iterator) \
	for (_vector_type::const_iterator _iterator = (_vector).cend(); _iterator != (_vector).cbegin(); --_iterator)


CLOSE_O2_NAMESPACE

#endif // DATA_UTILS_H
