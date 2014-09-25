#ifndef CONTAINERS_UTILS_H
#define CONTAINERS_UTILS_H

#include "util/public_namespace.h"
#include "array.h"

OPEN_O2_NAMESPACE

#define foreach(_type, _enumerable, _iterator) \
	for (IEnumerable< _type >::iterator _iterator = _enumerable.begin(); _iterator.index() < _enumerable.count(); ++_iterator)
	
CLOSE_O2_NAMESPACE

#endif // CONTAINERS_UTILS_H
