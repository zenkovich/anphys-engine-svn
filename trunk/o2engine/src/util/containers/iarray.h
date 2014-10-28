#ifndef ENUMERABLE_H
#define ENUMERABLE_H

#include "engine_settings.h"
#include "util/public_namespace.h"
#include "util/assert.h"
#include "util/compare.h"

OPEN_O2_NAMESPACE

/** Array interface. */
template<typename _type>
class IArray
{
public:
	virtual IArray* clone() const = 0;

	virtual _type& get(int idx) const = 0;

	virtual void set(int idx, const _type& value) = 0;

	virtual int count() const = 0;

	virtual void resize(int newCount) = 0;

	virtual _type& add(const _type& value) = 0;

	virtual _type popBack() = 0;

	virtual _type& insert(const _type& value, int position) = 0;

	virtual bool remove(int idx) = 0;

	virtual bool removeRange(int begin, int end) = 0;

	virtual bool remove(const _type& value) = 0;

	virtual void clear() = 0;

	virtual bool contains(const _type& value) const = 0;

	virtual int find(const _type& value) const = 0;

	virtual void sort(bool (*compareFunc)(_type&, _type&) = &stdCompareLess) = 0;

	_type& operator[](int idx);	

	virtual _type& first();

	virtual _type& last();

	virtual bool isEmpty() const;
};


template<typename _type>
_type& IArray<_type>::first()
{
	return get(0);
}

template<typename _type>
_type& IArray<_type>::last()
{
	return get(count() - 1);
}

template<typename _type>
_type& IArray<_type>::operator[](int idx)
{
	return get(idx);
}

template<typename _type>
bool IArray<_type>::isEmpty() const
{
	return count() == 0;
}

CLOSE_O2_NAMESPACE

#endif //ENUMERABLE_H
