#ifndef ENUMERABLE_H
#define ENUMERABLE_H

#include "public_namespace.h"

OPEN_O2_NAMESPACE

template<typename _type>
class IEnumerable
{
public:
	virtual int count() const = 0;
	virtual _type& get(int idx) = 0;
	virtual void set(int idx, const _type& value) = 0;
	virtual void remove(int idx) = 0;

	_type& operator[](int idx)
	{
		return get(idx);
	}
};

CLOSE_O2_NAMESPACE

#endif //ENUMERABLE_H
