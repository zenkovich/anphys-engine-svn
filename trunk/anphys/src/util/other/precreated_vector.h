#ifndef PRECREATED_ARRAY_H
#define PRECREATED_ARRAY_H

#include <vector>
#include <algorithm>

#include "../memory/mem_utils.h"

template<typename T>
struct cArray
{
	typedef std::vector<T*> ValuesList;

	ValuesList mValues;
	ValuesList mFreeValues;

	cArray(unsigned int reserveCount = 10)
	{
		alloc(reserveCount);
	}

	~cArray()
	{
		for (ValuesList::iterator it = mValues.begin(); it != mValues.end(); it++)
			safe_release(*it);

		for (ValuesList::iterator it = mFreeValues.begin(); it != mFreeValues.end(); it++)
			safe_release(*it);
	}

	void alloc(unsigned int count)
	{
		for (unsigned int i = 0; i < count; i++)
			mFreeValues.push_back(new T);
	}

	T* push_back()
	{
		T* newValue = getFreeValue();
		acceptValue(newValue);

		return newValue;
	}

	T* getFreeValue()
	{
		T* newValue = NULL;
		if (mFreeValues.size() > 0)
		{
			ValuesList::iterator lastElement = mFreeValues.begin();
			newValue = *lastElement;
		}
		else
		{
			newValue = new T;
		}

		return newValue;
	}

	void acceptValue(T* value)
	{
		if (mFreeValues.size() > 0)
		{
			ValuesList::iterator itElement = mFreeValues.begin();
			if (*itElement != value) 
			{
				itElement = std::find(mFreeValues.begin(), mFreeValues.end(), value);
			}

			if (itElement != mFreeValues.end()) mFreeValues.erase(itElement);
		}

		mValues.push_back(value);
	}

	bool erase(typename ValuesList::iterator& item)
	{
		if (item == end()) return false;

		mFreeValues.push_back(*item);
		mValues.erase(item);

		return true;
	}

	typename ValuesList::iterator& begin() { return mValues.begin(); }
	typename ValuesList::iterator& end() { return mValues.end(); }
	unsigned int size() { return mValues.size(); }

	void clear() 
	{
		for (ValuesList::iterator it = mValues.begin(); it != mValues.end(); it++)
			mFreeValues.push_back(*it);

		mValues.clear();
	}
};

template<typename T>
struct pvec
{
	T**  mFreeValues;
	T**  mValues;
		 
	int  mValuesCount;
	int  mFreeValuesCount;
	int  mSize;
	bool mGettedValue;

	pvec(int freeValues = 5)
	{
		mFreeValues = mValues = 0;
		mValuesCount = mFreeValuesCount = 0;
		mSize = 0;
		mGettedValue = false;
		resize(max(0, freeValues));
	}

	pvec(const pvec<T>& vec)
	{
		mFreeValues = mValues = 0;
		mValuesCount = mFreeValuesCount = 0;
		mGettedValue = false;
		resize(vec.mFreeValuesCount + vec.mValuesCount);

		for (int i = 0; i < vec.size(); i++)
		{
			*addValue() = vec[i];
		}
	}

	~pvec()
	{
		for (int i = 0; i < mValuesCount; i++)
			safe_release(mValues[i]);
		
		for (int i = 0; i < mFreeValuesCount; i++)
			safe_release(mFreeValues[i]);

		safe_release_arr(mValues);
		safe_release_arr(mFreeValues);
	}

	T* getFreeValue()
	{
		if (mGettedValue)
			return NULL;

		if (mFreeValuesCount == 0)
			resize(mSize + 5);

		mGettedValue = true;

		return mFreeValues[mFreeValuesCount - 1];
	}

	T* addValue()
	{
		T* newValue = getFreeValue();
		acceptValue(newValue);
		return newValue;
	}

	void acceptValue(T* value)
	{
		if (!mGettedValue)
			return;

		mGettedValue = false;

		mValues[mValuesCount++] = value;
		mFreeValuesCount--;
	}

	void removeValue(int idx)
	{
		if (mValuesCount == 0)
			return;

		idx = max(0, min(idx, mValuesCount - 1));

		mFreeValues[mFreeValuesCount++] = mValues[idx];

		T** lastValuePtr = &mValues[idx];
		for (int i = idx + 1; i < mValuesCount; i++)
		{
			*lastValuePtr = mValues[i];
			lastValuePtr = &mValues[i];
		}

		mValuesCount--;
	}

	void removeValue(T* value)
	{
		for (int i = 0; i < mValuesCount; i++)
			if (mValues[i] == value)
				removeValue(i);
	}

	void clear()
	{
		for (int i = 0; i < mValuesCount; i++)
		{
			mFreeValues[mFreeValuesCount++] = mValues[i];
		}

		mValues = 0;
	}

	void resize(int newSize)
	{
		if (mFreeValues)
		{
			for (int i = newSize; i < mFreeValuesCount; i++)
				safe_release(mFreeValues[i]);

			if (mFreeValuesCount > newSize)
				mFreeValuesCount = newSize;

			for (int i = newSize; i < mValuesCount; i++)
				safe_release(mValues[i]);

			if (mValuesCount > newSize)
				mValuesCount = newSize;
		
			T** buf = new T*[mSize];

			memcpy(buf, mFreeValues, sizeof(T*)*mFreeValuesCount);
			safe_release_arr(mFreeValues);
			mFreeValues = new T*[newSize];
			memcpy(mFreeValues, buf, sizeof(T*)*mFreeValuesCount);

			memcpy(buf, mValues, sizeof(T*)*mValuesCount);
			safe_release_arr(mValues);
			mValues = new T*[newSize];
			memcpy(mValues, buf, sizeof(T*)*mValuesCount);

			safe_release_arr(buf);
		}
		else
		{
			mFreeValues = new T*[newSize];
			mValues = new T*[newSize];
		}

		for (int i = 0; i < newSize - mSize; i++)
			mFreeValues[mFreeValuesCount++] = new T;

		mSize = newSize;
	}

	int size() const
	{
		return mValuesCount;
	}

	T& operator[](int idx) const
	{
		return *mValues[max(min(idx, mValuesCount - 1), 0)];
	}

	void operator=(const pvec<T>& vec)
	{
		clear();
		resize(vec.mFreeValuesCount + vec.mValuesCount);

		for (int i = 0; i < vec.size(); i++)
		{
			*addValue() = vec[i];
		}
	}
};

#endif //PRECREATED_ARRAY_H