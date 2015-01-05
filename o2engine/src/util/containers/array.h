#ifndef CONTAINER_ARRAY_H
#define CONTAINER_ARRAY_H

#include "util/containers/iarray.h"
#include "util/assert.h"
#include "util/mem_utils/memory_manager.h"
#include "util/mem_utils/alloc_operators.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
class array: public IArray<_type>
{
public:
	/** Array iterator. */
	class iterator
	{
		array* mArray;
		int    mIndex;

	public:
		iterator(array* arr = NULL, int index = 0);

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
	
	/** Array const iterator. */
	class citerator
	{
		const array* const mArray;
		int    mIndex;

	public:
		citerator(const array* const arr = NULL, int index = 0);

		int          index() const;
		const _type& value() const;
		
		citerator operator+(int offs);
		citerator operator-(int offs);
		
		citerator& operator++();
        citerator operator++(int);
		citerator& operator--();
        citerator operator--(int);
		
		citerator operator+=(int offs);	
		citerator operator-=(int offs);

		bool operator>(const citerator& itr);
		bool operator<(const citerator& itr);
		bool operator>=(const citerator& itr);
		bool operator<=(const citerator& itr);

		operator bool() const;

		const _type* const operator->();
		const _type&       operator*();
	};

protected:
	(_type*) mValues;
	int      mCount;
	int      mCapacity;

public:
	array(int capacity = 5);

	array(const array& arr);

	array(const IArray<_type>* arr);

	virtual ~array();

	array& operator=(const array& arr);

	array operator+(const array& arr) const;

	array operator+=(const array& arr);

	bool operator==(const array& arr);

	bool operator!=(const array& arr);

	IArray* clone() const;

	int count() const;

	int capacity() const;

	void resize(int newCount);

	void reserve(int newCapacity);

	_type& get(int idx) const;

	void set(int idx, const _type& value);

	_type& add(const _type& value);

	void add(const IArray& arr);

	_type popBack();

	_type& insert(const _type& value, int position);

	void insert(const IArray& arr, int position);

	int find(const _type& value) const;

	bool contains(const _type& value) const;

	bool removeAt(int idx);

	bool removeRange(int begin, int end);

	bool remove(const _type& value);

	iterator remove(const iterator& it);

	void clear();
	
	void sort(bool (*compareFunc)(_type&, _type&) = &stdCompareLess);

	iterator begin();

	iterator end();
	
	citerator cbegin() const;
	
	citerator cend() const;

protected:
	int getReservingSize(int size);

	void quickSort(bool (*compareFunc)(_type&, _type&), int left, int right);
};

//implementation array::iterator
template<typename _type>
array<_type>::iterator::iterator(array<_type>* arr = NULL, int index = 0) :
	mArray(arr), mIndex(index)
{
}
	
template<typename _type>
int array<_type>::iterator::index() const
{
	return mIndex;
}

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator+(int offs)
{
	return iterator(mArray, mIndex + offs);
}

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator-(int offs)
{
	return iterator(mArray, mIndex - offs);
}

template<typename _type>
typename array<_type>::iterator& array<_type>::iterator::operator++() // ++A;
{
	mIndex++;
	return *this ;
}

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator++(int) // A++;
{
	iterator temp = *this;
	mIndex++;
	return temp ;
}

template<typename _type>
typename array<_type>::iterator& array<_type>::iterator::operator--() // --A;
{
	mIndex--;
	return *this ;
}

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator--(int) // A--;		
{
	iterator temp = *this;
	mIndex--;
	return temp ;
}

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator+=(int offs)
{
	*this = *this + offs; 
	return *this;
}	

template<typename _type>
typename array<_type>::iterator array<_type>::iterator::operator-=(int offs)
{
	*this = *this - offs; 
	return *this;
}	

template<typename _type>
bool array<_type>::iterator::operator>(const iterator& itr)
{
	return mIndex > itr->mIndex;
}

template<typename _type>
bool array<_type>::iterator::operator<(const iterator& itr)
{
	return mIndex < itr->mIndex;
}

template<typename _type>
bool array<_type>::iterator::operator>=(const iterator& itr)
{
	return mIndex >= itr->mIndex;
}

template<typename _type>
bool array<_type>::iterator::operator<=(const iterator& itr)
{
	return mIndex <= itr->mIndex;
}

template<typename _type>
array<_type>::iterator::operator bool()
{
	return mIndex >= 0 && mIndex < mArray->count();
}

template<typename _type>
_type* array<_type>::iterator::operator->()
{
	return &value();
}

template<typename _type>
_type& array<_type>::iterator::operator*()
{
	return value();
}

template<typename _type>
_type& array<_type>::iterator::value() 
{
	if (CONTAINERS_DEBUG)
		o2assert(mArray && mIndex >= 0 && mIndex < mArray->count(), "Failed to get value iterator: index out of range"); 

	return mArray->get(mIndex);
}


//implementation array::citerator
template<typename _type>
array<_type>::citerator::citerator(const array<_type>* const arr = NULL, int index = 0) :
	mArray(arr), mIndex(index)
{
}
	
template<typename _type>
int array<_type>::citerator::index() const
{
	return mIndex;
}

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator+(int offs)
{
	return citerator(mArray, mIndex + offs);
}

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator-(int offs)
{
	return citerator(mArray, mIndex - offs);
}

template<typename _type>
typename array<_type>::citerator& array<_type>::citerator::operator++() // ++A;
{
	mIndex++;
	return *this ;
}

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator++(int) // A++;
{
	iterator temp = *this;
	mIndex++;
	return temp ;
}

template<typename _type>
typename array<_type>::citerator& array<_type>::citerator::operator--() // --A;
{
	mIndex--;
	return *this ;
}

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator--(int) // A--;		
{
	citerator temp = *this;
	mIndex--;
	return temp ;
}

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator+=(int offs)
{
	*this = *this + offs; 
	return *this;
}	

template<typename _type>
typename array<_type>::citerator array<_type>::citerator::operator-=(int offs)
{
	*this = *this - offs; 
	return *this;
}	

template<typename _type>
bool array<_type>::citerator::operator>(const citerator& itr)
{
	return mIndex > itr->mIndex;
}

template<typename _type>
bool array<_type>::citerator::operator<(const citerator& itr)
{
	return mIndex < itr->mIndex;
}

template<typename _type>
bool array<_type>::citerator::operator>=(const citerator& itr)
{
	return mIndex >= itr->mIndex;
}

template<typename _type>
bool array<_type>::citerator::operator<=(const citerator& itr)
{
	return mIndex <= itr->mIndex;
}

template<typename _type>
array<_type>::citerator::operator bool() const
{
	return mIndex >= 0 && mIndex < mArray->count();
}

template<typename _type>
const _type* const array<_type>::citerator::operator->()
{
	return &value();
}

template<typename _type>
const _type& array<_type>::citerator::operator*()
{
	return value();
}

template<typename _type>
const _type& array<_type>::citerator::value() const
{
	if (CONTAINERS_DEBUG)
		o2assert(mArray && mIndex >= 0 && mIndex < mArray->count(), "Failed to get value iterator: index out of range"); 

	return mArray->get(mIndex);
}


//implementation array
template<typename _type>
array<_type>::array(int capacity = 5)
{
	if (CONTAINERS_DEBUG)
		o2assert(capacity > 0, "Can't initialize array with empty capacity");

	mValues = (_type*)malloc(sizeof(_type)*capacity);
	mCapacity = capacity;
	mCount = 0;
}

template<typename _type>
array<_type>::array(const array& arr)
{
	mValues = (_type*)malloc(sizeof(_type)*arr.mCapacity);
	mCapacity = arr.mCapacity;
	mCount = arr.mCount;

	for (int i = 0; i < mCount; i++) 
		new (mValues + i) _type(arr.mValues[i]);
}
	
template<typename _type>
array<_type>::array(const IArray<_type>* arr)
{
	mCount = arr->count();
	mCapacity = getReservingSize(mCount);
	mValues = (_type*)malloc(sizeof(_type)*mCapacity);

	for (int i = 0; i < mCount; i++)
		new (mValues + i) _type(arr->get(i));
}

template<typename _type>
array<_type>::~array()
{
	clear();
	free(mValues);
}

template<typename _type>
IArray<_type>* array<_type>::clone() const
{
	return mnew array<_type>(this);
}

template<typename _type>
array<_type>& array<_type>::operator=(const array<_type>& arr)
{
	reserve(arr.mCapacity);
	mCount = arr.mCount;

	for (int i = 0; i < arr.mCount; i++)
	{
		if (i < mCount)
			mValues[i] = arr.mValues[i];
		else
			new (mValues + i) _type(arr.mValues[i]);
	}

	return *this;
}

template<typename _type>
array<_type> array<_type>::operator+(const array<_type>& arr) const
{ 
	array<_type> res(*this);
	res.add(arr);
	return res;
}

template<typename _type>
array<_type> array<_type>::operator+=(const array<_type>& arr) 
{
	add(arr);
	return *this;
}

template<typename _type>
bool array<_type>::operator==(const array<_type>& arr)
{
	if (arr.mCount != mCount)
		return false;

	for (int i = 0; i < mCount; i++)
		if (mValues[i] != arr.mValues[i])
			return false;

	return true;
}

template<typename _type>
bool array<_type>::operator!=(const array<_type>& arr)
{
	return !(*this == arr);
}

template<typename _type>
int array<_type>::count() const
{
	return  mCount;
}

template<typename _type>
int array<_type>::capacity() const
{
	return mCapacity;
}

template<typename _type>
void array<_type>::resize(int newCount)
{
	if (CONTAINERS_DEBUG)
		o2assert(newCount > 0, "Can't resize array to zero size");

	reserve(getReservingSize(newCount));

	if (mCount < newCount)
		for (int i = newCount; i < mCount; i++)
			mValues[i].~_type();
	else
		for (int i = mCount; i < newCount; i++)
			new (mValues + i) _type();

	mCount = newCount;
}

template<typename _type>
void array<_type>::reserve(int newCapacity)
{
	if (CONTAINERS_DEBUG)
		o2assert(newCapacity > 0, "Can't reserve array to zero size");

	if (newCapacity < mCapacity)
		newCapacity = mCapacity;

	if (newCapacity < 5)
		newCapacity = 5;

	mCapacity = newCapacity;

	mValues = (_type*)realloc(mValues, mCapacity*sizeof(_type));
}

template<typename _type>
_type& array<_type>::get(int idx) const
{
	if (CONTAINERS_DEBUG)
		o2assert(idx >= 0 || idx < mCount, "Can't get array element: index out of range");

	return mValues[idx];
}

template<typename _type>
void array<_type>::set(int idx, const _type& value)
{
	if (CONTAINERS_DEBUG)
		o2assert(idx >= 0 || idx < mCount, "Can't set array element: index out of range");

	mValues[idx] = value;
}

template<typename _type>
_type& array<_type>::add(const _type& value)
{
	if (mCount == mCapacity)
		reserve(getReservingSize(mCount + 1));

	new (mValues + mCount) _type(value);
	mCount++;

	return mValues[mCount - 1];
}

template<typename _type>	
void array<_type>::add(const IArray<_type>& arr)
{
	int arrCount = arr.count();
	if (mCount + arrCount >= mCapacity)
		reserve(getReservingSize(mCount + arrCount));

	for (int i = 0; i < arrCount; i++)
		new (mValues + mCount + i) _type(arr.get(i));

	mCount += arrCount;
}

template<typename _type>
_type array<_type>::popBack()
{
	if (CONTAINERS_DEBUG)
		o2assert(mCount > 0 ,"Can't pop value from array: no values");

	mCount--;
	_type res = mValues[mCount];
	mValues[mCount].~_type();
	return res;
}

template<typename _type>
_type& array<_type>::insert(const _type& value, int position)
{
	if (CONTAINERS_DEBUG)
		o2assert(position >= 0 || position < mCount ,"Can't insert element: index out of range");

	if (mCount == mCapacity)
		reserve(getReservingSize(mCount + 1));
	
	new (mValues + mCount) _type();
	mCount++;

	_type tmp = value;
	for (int i = position; i < mCount; i++)
	{
		_type curValue = mValues[i];
		mValues[i] = tmp;
		tmp = curValue;
	}

	return mValues[position];
}

template<typename _type>
void array<_type>::insert(const IArray<_type>& arr, int position)
{
	int arrCount = arr.count();
	if (mCount + arrCount >= mCapacity)
		reserve(getReservingSize(mCount + arrCount));

	for (int i = position; i < mCount; i++)
		new (mValues + i + arrCount) _type(mValues[i]);

	for (int i = 0; i < arrCount; i++)
	{
		if (i < mCount)
			mValues[i + position] = arr.get(i);
		else
			new (mValues + i + position) _type(arr.get(i));
	}

	mCount += arrCount;
}

template<typename _type>
int array<_type>::find(const _type& value) const
{
	for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return i;

	return -1;
}

template<typename _type>
bool array<_type>::contains(const _type& value) const
{
	for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return true;

	return false;
}

template<typename _type>
bool array<_type>::removeAt(int idx)
{
	if (idx < 0 || idx >= mCount)
		return false;

	mValues[idx].~_type();

	for (int i = idx; i < mCount - 1; i++)
		mValues[i] = mValues[i + 1];

	mCount--;

	return true;
}

template<typename _type>
bool array<_type>::removeRange(int begin, int end)
{
	if (begin < 0 || end < 0 || begin >= mCount || end >= mCount || begin > end)
		return false;

	int diff = end - begin;

	for (int i = begin; i < end; i++)
		mValues[i].~_type();

	for (int i = begin; i < mCount - diff; i++)
		mValues[i] = mValues[i + diff];

	mCount -= diff;

	return true;
}

template<typename _type>
bool array<_type>::remove(const _type& value)
{
	int idx = find(value);
	if (idx < 0)
		return false;

	removeAt(idx);
	return true;
}

template<typename _type>
typename array<_type>::iterator array<_type>::remove( const iterator& it )
{
	removeAt(it.index());
	return it;
}

template<typename _type>
void array<_type>::clear()
{
	mCount = 0;
}

template<typename _type>	
void array<_type>::sort(bool (*compareFunc)(_type&, _type&))
{
	quickSort(compareFunc, 0, mCount - 1);
}

template<typename _type>
int array<_type>::getReservingSize(int size)
{
	return (int)((float)size*1.5f);
}

template<typename _type>
void array<_type>::quickSort(bool (*compareFunc)(_type&, _type&), int left, int right)		
{
	int i = left, j = right;
	_type tmp;
	_type pivot = mValues[(left + right)/2];

	/* partition */
	while (i <= j) 
	{
		while ( compareFunc(mValues[i], pivot) )
			i++;

		while ( compareFunc(pivot, mValues[j])  )
			j--;

		if (i <= j)
		{
			tmp = mValues[i];
			mValues[i] = mValues[j];
			mValues[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(compareFunc, left, j);
	if (i < right)
		quickSort(compareFunc, i, right);
}

template<typename _type>
typename array<_type>::iterator array<_type>::begin()
{
	return iterator(this, 0);
}

template<typename _type>
typename array<_type>::iterator array<_type>::end()
{
	return iterator(this, count() - 1);
}
	
template<typename _type>
typename array<_type>::citerator array<_type>::cbegin() const
{
	return citerator(this, 0);
}
	
template<typename _type>
typename array<_type>::citerator array<_type>::cend() const
{
	return citerator(this, count() - 1);
}

CLOSE_O2_NAMESPACE

#endif //CONTAINER_ARRAY_H
