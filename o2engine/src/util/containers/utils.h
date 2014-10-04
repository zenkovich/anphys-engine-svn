#ifndef CONTAINERS_UTILS_H
#define CONTAINERS_UTILS_H

#include "util/public_namespace.h"
#include "array.h"

OPEN_O2_NAMESPACE

#define foreach(_container_type, _enumerable, _iterator) \
	for (_container_type::iterator _iterator = _enumerable.begin(); _iterator.index() < _enumerable.count(); ++_iterator)
	
CLOSE_O2_NAMESPACE

#endif // CONTAINERS_UTILS_H
