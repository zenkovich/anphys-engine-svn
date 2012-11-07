#ifndef PRECREATED_ARRAY_H
#define PRECREATED_ARRAY_H

#include <vector>

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
		T* newValue = NULL;
		if (mFreeValues.size() > 0)
		{
			ValuesList::iterator lastElement = mFreeValues.end() - 1;
			newValue = *lastElement;
			mFreeValues.erase(lastElement);
		}
		else
		{
			newValue = new T;
		}

		mValues.push_back(newValue);

		return newValue;
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

	typename ValuesList::iterator& iterator() { return ValuesList::iterator; }

	void clear() 
	{
		for (ValuesList::iterator it = mValues.begin(); it != mValues.end(); it++)
			mFreeValues.push_back(*it);

		mValues.clear();
	}
};

#endif //PRECREATED_ARRAY_H