#ifndef CONTAINER_ARRAY_H
#define CONTAINER_ARRAY_H

#include "util/containers/enumerable.h"
#include "util/assert.h"

OPEN_O2_NAMESPACE

template<typename _type>
class array: public IEnumerable<_type>
{
	(_type*) mValues;
	int    mCount;
	int    mCapacity;

public:
	array(int capacity = 5)
	{
		if (CONTAINERS_DEBUG)
			o2assert(capacity > 0, "Can't initialize array with empty capacity");

		mValues = new _type[capacity];
		mCapacity = capacity;
		mCount = 0;
	}

	array(const array& arr)
	{
		mValues = new _type[arr.mCapacity];
		mCapacity = arr.mCapacity;
		mCount = arr.mCount;

		for (int i = 0; i < mCount; i++)
			mValues[i] = arr.mValues[i];
	}

	virtual ~array()
	{
		clear();
		delete[] mValues;
	}

	array& operator=(const array& arr)
	{
		reserve(arr.mCapacity);
		mCount = arr.mCount;

		for (int i = 0; i < mCount; i++)
			mValues[i] = arr.mValues[i];

		return *this;
	}

	bool operator==(const array& arr)
	{
		if (arr.mCount != mCount)
			return false;

		for (int i = 0; i < mCount; i++)
			if (mValues[i] != arr.mValues[i])
				return false;

		return true;
	}

	bool operator!=(const array& arr)
	{
		return !(*this == arr);
	}

	int count() const
	{
		return  mCount;
	}

	bool isEmpty() const
	{
		return mCount == 0;
	}

	int capacity() const
	{
		return mCapacity;
	}

	void resize(int newCount)
	{
		if (CONTAINERS_DEBUG)
			o2assert(newCount > 0, "Can't resize array to zero size");

		reserve(getReservingSize(newCount));
		mCount = newCount;
	}

	void reserve(int newCapacity)
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

	_type& get(int idx)
	{
		if (CONTAINERS_DEBUG)
			o2assert(idx < 0 || idx >= mCount ,"Can't get array element: index out of range");

		return mValues[idx];
	}

	void set(int idx, const _type& value)
	{
		if (CONTAINERS_DEBUG)
			o2assert(idx < 0 || idx >= mCount ,"Can't set array element: index out of range");

		mValues[idx] = value;
	}

	_type& add(const _type& value)
	{
		if (mCount == mCapacity)
			reserve(getReservingSize(mCount));

		mValues[mCount++] = value;

		return mValues[mCount - 1];
	}

	_type& popBack()
	{
		if (CONTAINERS_DEBUG)
			o2assert(mCount > 0 ,"Can't pop value from array: no values");

		mCount--;
		return mValues[mCount];
	}

	_type& insert(const _type& value, int position)
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

	int find(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
			if (mValues[i] == value)
				return i;

		return -1;
	}

	bool contains(const _type& value) const
	{
		for (int i = 0; i < mCount; i++)
			if (mValues[i] == value)
				return true;

		return false;
	}

	void remove(int idx)
	{
		if (CONTAINERS_DEBUG)
			o2assert(idx < 0 || idx >= mCount ,"Can't remove element: index out of range");

		for (int i = idx; i < mCount - 1; i++)
			mValues[i] = mValues[i + 1];

		mCount--;
	}

	void removeRange(int begin, int end)
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

	bool remove(const _type& value)
	{
		int idx = find(value);
		if (idx < 0)
			return false;

		remove(idx);
		return true;
	}

	void clear()
	{
		mCount = 0;
	}
	
	void sort(bool (*compareFunc)(_type&, _type&))
	{
		quickSort(compareFunc, 0, mCount - 1);
	}

	void sort()
	{
		sort(&stdComparer);
	}

protected:
	int getReservingSize(int size)
	{
		return (int)((float)size*1.5f);
	}

	void quickSort(bool (*compareFunc)(_type&, _type&), int left, int right)		
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

	template<typename _ct>
	static bool stdComparer(_ct& a, _ct& b) { return a < b; }
};

CLOSE_O2_NAMESPACE

#endif //CONTAINER_ARRAY_H
