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

#endif //PRECREATED_ARRAY_H