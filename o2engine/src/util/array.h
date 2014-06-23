#ifndef ARRAY_H
#define ARRAY_H

#include "enumerable.h"
#include "assert.h"

#define ENABLE_DATA_CHECKS

#ifdef ENABLE_DATA_CHECKS
#define isDataChecksEnabled() true
#else
#define isDataChecksEnabled() false
#endif

OPEN_O2_NAMESPACE

template<typename _type>
class array: public IEnumerable<_type>
{
	_type* mValues;
	int    mCount;
	int    mCapacity;

public:
	array(int capacity = 4)
	{
		if (isDataChecksEnabled())
			assert(capacity > 0, "Can't initialize array with empty capacity");

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
		mCapacity = arr.mCapacity;
		mCount = arr.mCount;

		for (int i = 0; i < mCount; i++)
			mValues[i] = arr.mValues[i];
	}

	int count() const
	{
		return  mCount;
	}

	int capacity() const
	{
		return mCapacity;
	}

	void resize(int newCount)
	{
		if (isDataChecksEnabled())
			assert(newCount > 0, "Can't resize array to zero size");

		reserve(getReservingSize(newCount));
		mCount = newCount;
	}

	void reserve(int newCapacity)
	{
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
	}

	_type& get(int idx)
	{
		if (isDataChecksEnabled())
			assert(idx < 0 || idx >= mCount ,"Can't get array element: index out of range");

		return mValues[idx];
	}

	void set(int idx, const _type& value)
	{
		if (isDataChecksEnabled())
			assert(idx < 0 || idx >= mCount ,"Can't set array element: index out of range");

		mValues[idx] = value;
	}

	_type& add(const _type& value)
	{
		if (mCount == mCapacity)
			reserve(getReservingSize(mCount));

		mValues[mCount++] = value;

		return value;
	}

	_type& insert(const _type& value, int position)
	{
		if (isDataChecksEnabled())
			assert(idx < 0 || idx >= mCount ,"Can't insert element: index out of range");

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

	int find(const _type& value)
	{
		for (int i = 0; i < mCount; i++)
			if (mValues[i] == value)
				return i;

		return -1;
	}

	bool contains(const _type& value)
	{
		for (int i = 0; i < mCount; i++)
			if (mValues[i] == value)
				return true;

		return false;
	}

	void remove(int idx)
	{
		if (isDataChecksEnabled())
			assert(idx < 0 || idx >= mCount ,"Can't remove element: index out of range");

		for (int i = idx; i < mCount - 1; i++)
			mValues[i] = mValues[i + 1];

		mCount--;
	}

	void removeRange(int begin, int end)
	{
		if (isDataChecksEnabled())
		{
			assert(begin < 0 || begin >= mCount || end < 0 || end >= mCount || end < begin, 
			       "Can't remove elements: indexes out of range");
		}

		int diff = end - begin;

		for (int i = begin; i < mCount - diff; i++)
			mValues[i] = mValues[i + diff];

		mCount -= diff;
	}

	void remove(const _type& value)
	{
		remove(find(value));
	}

	void clear()
	{
		mCount = 0;
	}

protected:
	int getReservingSize(int size) 
	{
		return (int)((float)size*1.5f);
	}
};

CLOSE_O2_NAMESPACE

#endif //ARRAY_H
