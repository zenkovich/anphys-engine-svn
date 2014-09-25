#include "array.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
int array<_type>::getReservingSize(int size)
{
	return (int)((float)size*1.5f);
}

template<typename _type>
void array<_type>::quickSort(void (*compareFunc)(_type&, _type&), int left, int right)
{
	int i = left, j = right;
	_type tmp;
	_type pivot = mValues[(left + right)/2];

	/* partition */
	while (i <= j) 
	{
		while ( compareFunc(mValues[i], pivot) )
			i++;

		while ( !compareFunc(mValues[j], pivot) )
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
void array<_type>::sort(void (*compareFunc)(_type&, _type&))
{
	quickSort(0, mCount);
}

template<typename _type>
void array<_type>::sort()
{
	sort(&stdComparer);
}

template<typename _type>
void array<_type>::clear()
{
	mCount = 0;
}

template<typename _type>
bool array<_type>::remove(const _type& value)
{
	int idx = find(value);
	if (idx < 0)
		return false;

	remove(idx);
	return true;
}

template<typename _type>
void array<_type>::remove(int idx)
{
	if (CONTAINERS_DEBUG)
		o2assert(idx < 0 || idx >= mCount ,"Can't remove element: index out of range");

	for (int i = idx; i < mCount - 1; i++)
		mValues[i] = mValues[i + 1];

	mCount--;
}

template<typename _type>
void array<_type>::removeRange(int begin, int end)
{
	if (CONTAINERS_DEBUG)
	{
		o2assert(begin < 0 || begin >= mCount || end < 0 || end >= mCount || end < begin, 
			"Can't remove elements: indexes out of range");
	}

	int diff = end - begin;

	for (int i = begin; i < mCount - diff; i++)
		mValues[i] = mValues[i + diff];

	mCount -= diff;
}

template<typename _type>
bool array<_type>::contains(const _type& value)
{
	for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return true;

	return false;
}

template<typename _type>
int array<_type>::find(const _type& value)
{
	for (int i = 0; i < mCount; i++)
		if (mValues[i] == value)
			return i;

	return -1;
}

template<typename _type>
_type& array<_type>::insert(const _type& value, int position)
{
	if (CONTAINERS_DEBUG)
		o2assert(idx < 0 || idx >= mCount ,"Can't insert element: index out of range");

	if (mCount == mCapacity)
		reserve(getReservingSize(mCount));

	mCount++;

	_type tmp = value;
	for (int i = position; i < mCount; i++)
	{
		_type curValue = mValues[i];
		mValues[i] = tmp;
		tmp = curValue;
	}

	return value;
}

template<typename _type>
_type& array<_type>::popBack()
{
	if (CONTAINERS_DEBUG)
		o2assert(mCount > 0 ,"Can't pop value from array: no values");

	mCount--;
	return mValues[mCount];
}

template<typename _type>
_type& array<_type>::add(const _type& value)
{
	if (mCount == mCapacity)
		reserve(getReservingSize(mCount));

	mValues[mCount++] = value;

	return mValues[mCount - 1];
}

template<typename _type>
void array<_type>::set(int idx, const _type& value)
{
	if (CONTAINERS_DEBUG)
		o2assert(idx < 0 || idx >= mCount ,"Can't set array element: index out of range");

	mValues[idx] = value;
}

template<typename _type>
_type& array<_type>::get(int idx)
{
	if (CONTAINERS_DEBUG)
		o2assert(idx < 0 || idx >= mCount ,"Can't get array element: index out of range");

	return mValues[idx];
}

template<typename _type>
void array<_type>::reserve(int newCapacity)
{
	if (CONTAINERS_DEBUG)
		o2assert(newCapacity > 0, "Can't reserve array to zero size");

	if (newCapacity < mCount)
		newCapacity = mCount;

	if (newCapacity < 5)
		newCapacity = 5;

	_type* tmp = new _type[mCount];

	for (int i = 0; i < mCount; i++)
		tmp[i] = mValues[i];

	delete[] mValues;
	mValues = new _type[newCapacity];

	for (int i = 0; i < mCount; i++)
		tmp[i] = mValues[i];

	delete[] tmp;
	mCapacity = newCapacity;
}

template<typename _type>
void array<_type>::resize(int newCount)
{
	if (CONTAINERS_DEBUG)
		o2assert(newCount > 0, "Can't resize array to zero size");

	reserve(getReservingSize(newCount));
	mCount = newCount;
}

template<typename _type>
int array<_type>::capacity() const
{
	return mCapacity;
}

template<typename _type>
bool array<_type>::isEmpty() const
{
	return mCount == 0;
}

template<typename _type>
int array<_type>::count() const
{
	return  mCount;
}

template<typename _type>
bool array<_type>::operator!=(const array& arr)
{
	return !(*this == arr);
}

template<typename _type>
bool array<_type>::operator==(const array& arr)
{
	if (arr.mCount != mCount)
		return false;

	for (int i = 0; i < mCount; i++)
		if (mValues[i] != arr.mValues[i])
			return false;

	return true;
}

template<typename _type>
array<_type>& array<_type>::operator=(const array& arr)
{
	reserve(arr.mCapacity);
	mCount = arr.mCount;

	for (int i = 0; i < mCount; i++)
		mValues[i] = arr.mValues[i];

	return *this;
}

template<typename _type>
array<_type>::~array()
{
	clear();
	delete[] mValues;
}

template<typename _type>
array<_type>::array(const array& arr)
{
	mValues = new _type[arr.mCapacity];
	mCapacity = arr.mCapacity;
	mCount = arr.mCount;

	for (int i = 0; i < mCount; i++)
		mValues[i] = arr.mValues[i];
}

template<typename _type>
array<_type>::array(int capacity /*= 5*/)
{
	if (CONTAINERS_DEBUG)
		o2assert(capacity > 0, "Can't initialize array with empty capacity");

	mValues = new _type[capacity];
	mCapacity = capacity;
	mCount = 0;
}

CLOSE_O2_NAMESPACE