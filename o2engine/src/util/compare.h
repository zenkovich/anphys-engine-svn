#ifndef COMPARE_H
#define COMPARE_H

#include "public_namespace.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
bool stdCompareLess(_type& a, _type& b) { return a < b; }

CLOSE_O2_NAMESPACE

#endif // COMPARE_H
