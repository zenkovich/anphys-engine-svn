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
		_type*       mObjPtr;

	public:
		iterator(IEnumerable* arr = NULL, int index = 0);

		int index() const;

		iterator operator+(int offs);
		iterator operator-(int offs);
		iterator& operator++(); // ++A;
        iterator operator++(int); // A++;
		iterator& operator--(); // --A;
        iterator operator--(int); // A--;		
		iterator operator+=(int offs);		
		iterator operator-=(int offs);		
		bool operator>(const iterator& itr);
		bool operator<(const iterator& itr);
		bool operator>=(const iterator& itr);
		bool operator<=(const iterator& itr);
		operator bool();
		operator _type*();
		_type* operator->();
		_type& operator*();
	};

public:
	virtual int count() const = 0;

	virtual _type& get(int idx) = 0;
	virtual _type& first();
	virtual _type& last();

	virtual void set(int idx, const _type& value) = 0;

	virtual bool contains(const _type& value) = 0;
	virtual int find(const _type& value) = 0;

	virtual iterator begin();
	virtual iterator end();

	_type& operator[](int idx);
};

CLOSE_O2_NAMESPACE

#endif //ENUMERABLE_H
