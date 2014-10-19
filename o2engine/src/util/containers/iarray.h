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
	/** Array iterator. */
	class iterator
	{
		IArray* mArray;
		int     mIndex;

	public:
		iterator(IArray* arr = NULL, int index = 0);

		int       index() const;
		_type&    value();

		iterator operator+(int offs);
		iterator operator-(int offs);

		iterator& operator++();
        iterator operator++(int);
		iterator& operator--();
        iterator operator--(int);

		iterator operator+=(int offs);	
		iterator operator-=(int offs);

		bool operator>(const iterator& itr);
		bool operator<(const iterator& itr);
		bool operator>=(const iterator& itr);
		bool operator<=(const iterator& itr);

		operator bool();

		_type* operator->();
		_type& operator*();
	};

public:
	virtual IArray* clone() const = 0;

	virtual _type& get(int idx) const = 0;

	virtual void set(int idx, const _type& value) = 0;

	virtual int count() const = 0;

	virtual void resize(int newCount) = 0;

	virtual _type& add(const _type& value) = 0;

	virtual _type& popBack() = 0;

	virtual _type& insert(const _type& value, int position) = 0;

	virtual bool remove(int idx) = 0;

	virtual bool removeRange(int begin, int end) = 0;

	virtual bool remove(const _type& value) = 0;

	virtual bool remove(const iterator& it);

	virtual void clear() = 0;

	virtual bool contains(const _type& value) const = 0;

	virtual int find(const _type& value) const = 0;

	virtual void sort(bool (*compareFunc)(_type&, _type&) = &stdCompareLess) = 0;

	_type& operator[](int idx);	

	virtual _type& first();

	virtual _type& last();

	virtual bool isEmpty() const;

	virtual iterator begin() const;

	virtual iterator end() const;
};

//implementation IEnumerable::iterator

template<typename _type>
IArray<_type>::iterator::iterator(IArray<_type>* arr = NULL, int index = 0) :
	mArray(arr), mIndex(index)
{
}
	
template<typename _type>
int IArray<_type>::iterator::index() const
{
	return mIndex;
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator+(int offs)
{
	return iterator(mArray, mIndex + offs);
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator-(int offs)
{
	return iterator(mArray, mIndex - offs);
}

template<typename _type>
typename IArray<_type>::iterator& IArray<_type>::iterator::operator++() // ++A;
{
	mIndex++;

//  if (CONTAINERS_DEBUG)
//  	o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	return *this ;
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator++(int) // A++;
{
	iterator temp = *this;

	mIndex++;

//  if (CONTAINERS_DEBUG)
//  	o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	return temp ;
}

template<typename _type>
typename IArray<_type>::iterator& IArray<_type>::iterator::operator--() // --A;
{
	mIndex--;

//  if (CONTAINERS_DEBUG)
//  	o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	return *this ;
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator--(int) // A--;		
{
	iterator temp = *this;

	mIndex--;

//  if (CONTAINERS_DEBUG)
//  	o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	return temp ;
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator+=(int offs)
{
	*this = *this + offs; 
	return *this;
}	

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::iterator::operator-=(int offs)
{
	*this = *this - offs; 
	return *this;
}	

template<typename _type>
bool IArray<_type>::iterator::operator>(const iterator& itr)
{
	return mIndex > itr->mIndex;
}

template<typename _type>
bool IArray<_type>::iterator::operator<(const iterator& itr)
{
	return mIndex < itr->mIndex;
}

template<typename _type>
bool IArray<_type>::iterator::operator>=(const iterator& itr)
{
	return mIndex >= itr->mIndex;
}

template<typename _type>
bool IArray<_type>::iterator::operator<=(const iterator& itr)
{
	return mIndex <= itr->mIndex;
}

template<typename _type>
IArray<_type>::iterator::operator bool()
{
	return mIndex >= 0 && mIndex < mArray->count();
}

template<typename _type>
_type* IArray<_type>::iterator::operator->()
{
	return &value();
}

template<typename _type>
_type& IArray<_type>::iterator::operator*()
{
	return value();
}

template<typename _type>
_type& IArray<_type>::iterator::value() 
{
	if (CONTAINERS_DEBUG)
		o2assert(mArray && mIndex >= 0 && mIndex < mArray->count(), "Failed to get value iterator: index out of range"); 

	return mArray->get(mIndex);
}

//implementation IEnumerable
template<typename _type>
bool IArray<_type>::remove( const iterator& it )
{
	return remove(it.index());
}


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
typename IArray<_type>::iterator IArray<_type>::begin() const
{
	return iterator(this, 0);
}

template<typename _type>
typename IArray<_type>::iterator IArray<_type>::end() const
{
	return iterator(this, count() - 1);
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
