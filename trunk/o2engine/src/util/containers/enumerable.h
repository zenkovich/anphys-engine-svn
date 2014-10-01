#ifndef ENUMERABLE_H
#define ENUMERABLE_H

#include "engine_settings.h"
#include "util/public_namespace.h"
#include "util/assert.h"

OPEN_O2_NAMESPACE

template<typename _type>
class IEnumerable
{
public:
	class iterator
	{
		IEnumerable* mArray;
		int          mIndex;

	public:
		iterator(IEnumerable* arr = NULL, int index = 0) :
			mArray(arr), mIndex(index)
		{
		}

		int index() const
		{
			return mIndex;
		}

		iterator operator+(int offs)
		{
			return iterator(mArray, mIndex + offs);
		}

		iterator operator-(int offs)
		{
			return iterator(mArray, mIndex - offs);
		}

		iterator& operator++() // ++A;
		{
			mIndex++;

// 			if (CONTAINERS_DEBUG)
// 				o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

			return *this ;
		}

        iterator operator++(int) // A++;
		{
			iterator temp = *this;

			mIndex++;

// 			if (CONTAINERS_DEBUG)
// 				o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

			return temp ;
		}

		iterator& operator--() // --A;
		{
			mIndex--;

// 			if (CONTAINERS_DEBUG)
// 				o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

			return *this ;
		}

        iterator operator--(int) // A--;		
		{
			iterator temp = *this;

			mIndex--;

// 			if (CONTAINERS_DEBUG)
// 				o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to increment iterator: index out of range");

			return temp ;
		}

		iterator operator+=(int offs)
		{
			*this = *this + offs; 
			return *this;
		}	

		iterator operator-=(int offs)
		{
			*this = *this - offs; 
			return *this;
		}	

		bool operator>(const iterator& itr)
		{
			return mIndex > itr->mIndex;
		}

		bool operator<(const iterator& itr)
		{
			return mIndex < itr->mIndex;
		}

		bool operator>=(const iterator& itr)
		{
			return mIndex >= itr->mIndex;
		}

		bool operator<=(const iterator& itr)
		{
			return mIndex <= itr->mIndex;
		}

		operator bool()
		{
			return mIndex >= 0 && mIndex < mArray->count();
		}

		operator _type*()
		{
			return &value();
		}

		_type* operator->()
		{
			return &value();
		}

		_type& operator*()
		{
			return value();
		}

		_type& value() 
		{
			if (CONTAINERS_DEBUG)
				o2assert(mIndex >= 0 && mIndex < mArray->count(), "Failed to get value iterator: index out of range"); 

			return mArray->get(mIndex);
		}
	};

public:
	virtual int count() const = 0;

	virtual _type& get(int idx) = 0;
	virtual _type& first()
	{
		return get(0);
	}

	virtual _type& last()
	{
		return get(count() - 1);
	}

	virtual void set(int idx, const _type& value) = 0;

	virtual bool contains(const _type& value) const = 0;
	virtual int find(const _type& value) const = 0;

	virtual iterator begin()
	{
		return iterator(this, 0);
	}

	virtual iterator end()
	{
		return iterator(this, count() - 1);
	}

	_type& operator[](int idx)
	{
		return get(idx);
	}
};

CLOSE_O2_NAMESPACE

#endif //ENUMERABLE_H
