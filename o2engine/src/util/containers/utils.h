#ifndef CONTAINERS_UTILS_H
#define CONTAINERS_UTILS_H

#include "util/public_namespace.h"
#include "array.h"

OPEN_O2_NAMESPACE
	
#define foreach(_container_type, _enumerable, _iterator) \
	for (_container_type::iterator _iterator = _enumerable.begin(); _iterator.index() < _enumerable.count(); ++_iterator)
	
#define foreach_back(_container_type, _enumerable, _iterator) \
	for (_container_type::iterator _iterator = _enumerable.end(); _iterator.index() >= 0; --_iterator)
	
#define foreach_const(_container_type, _enumerable, _iterator) \
	for (_container_type::citerator _iterator = _enumerable.cbegin(); _iterator.index() < _enumerable.count(); ++_iterator)
	
#define foreach_rem(_container_type, _enumerable, _iterator) \
	for (_container_type::iterator _iterator = _enumerable.begin(); _iterator.index() < _enumerable.count(); )

#define release_array(_vector_type, _enumerable) \
	for (_vector_type::iterator _iterator = _enumerable.begin(); _iterator.index() < _enumerable.count(); ++_iterator) \
		safe_release(*_iterator); \
	(_enumerable).clear()
	
CLOSE_O2_NAMESPACE

#endif // CONTAINERS_UTILS_H
