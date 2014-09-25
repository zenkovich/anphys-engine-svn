#include "enumerable.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
_type& IEnumerable<_type>::operator[](int idx)
{
	return get(idx);
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::end()
{
	return iterator(this, count() - 1);
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::begin()
{
	return iterator(this, 0);
}

template<typename _type>
_type& IEnumerable<_type>::last()
{
	return get(count() - 1);
}

template<typename _type>
_type& IEnumerable<_type>::first()
{
	return get(0);
}


template<typename _type>
_type& IEnumerable<_type>::iterator::operator*()
{
	return *mObjPtr;
}

template<typename _type>
_type* IEnumerable<_type>::iterator::operator->()
{
	return mObjPtr;
}

template<typename _type>
IEnumerable<_type>::iterator::operator _type*()
{
	return mObjPtr;
}

template<typename _type>
IEnumerable<_type>::iterator::operator bool()
{
	return mObjPtr != NULL;
}

template<typename _type>
bool IEnumerable<_type>::iterator::operator<=(const iterator& itr)
{
	return mIndex <= itr->mIndex;
}

template<typename _type>
bool IEnumerable<_type>::iterator::operator>=(const iterator& itr)
{
	return mIndex >= itr->mIndex;
}

template<typename _type>
bool IEnumerable<_type>::iterator::operator<(const iterator& itr)
{
	return mIndex < itr->mIndex;
}

template<typename _type>
bool IEnumerable<_type>::iterator::operator>(const iterator& itr)
{
	return mIndex > itr->mIndex;
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator-=(int offs)
{
	*this = *this - offs; 
	return *this;
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator+=(int offs)
{
	*this = *this + offs; 
	return *this;
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator--(int) // A--
{
	iterator temp = *this;

	mIndex--;

	if (CONTAINERS_DEBUG)
		o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	mObjPtr = &mArray->get(mIndex);

	return temp ;
}

template<typename _type>
typename IEnumerable<_type>::iterator& IEnumerable<_type>::iterator::operator--() // --A
{
	mIndex--;

	if (CONTAINERS_DEBUG)
		o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	mObjPtr = &mArray->get(mIndex);

	return *this ;
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator++(int) // A++
{
	iterator temp = *this;

	mIndex++;

	if (CONTAINERS_DEBUG)
		o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	mObjPtr = &mArray->get(mIndex);

	return temp ;
}

template<typename _type>
typename IEnumerable<_type>::iterator& IEnumerable<_type>::iterator::operator++() // ++A
{
	mIndex++;

	if (CONTAINERS_DEBUG)
		o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

	mObjPtr = &mArray->get(mIndex);

	return *this ;
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator-(int offs)
{
	return iterator(mArray, mIndex - offs);
}

template<typename _type>
typename IEnumerable<_type>::iterator IEnumerable<_type>::iterator::operator+(int offs)
{
	return iterator(mArray, mIndex + offs);
}

template<typename _type>
int IEnumerable<_type>::iterator::index() const
{
	return mIndex;
}

template<typename _type>
IEnumerable<_type>::iterator::iterator(IEnumerable* arr /*= NULL*/, int index /*= 0*/) :
mArray(arr), mIndex(index), mObjPtr(NULL)
{
	if (mArray)
	{
		if (CONTAINERS_DEBUG)
			o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to initialize iterator: index out of range"); 

		mObjPtr = &mArray->get(mIndex);
	}
}

CLOSE_O2_NAMESPACE