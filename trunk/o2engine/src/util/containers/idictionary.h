#ifndef IDICTIONARY_H
#define IDICTIONARY_H

#include "engine_settings.h"
#include "util/public_namespace.h"
#include "util/assert.h"

OPEN_O2_NAMESPACE

template<typename _key_type, typename _value_type>
class IDictionary
{
public:

	class iterator
	{
		IDictionary* mDictionary;
		int          mIndex;

	public:
		iterator(IDictionary* dictionary = NULL, int index = 0):mDictionary(dictionary), mIndex(index) {}

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

		_key_type& key() const;

		_value_type& value() const;
	};

public:
	virtual iterator begin();

	virtual iterator end();

	virtual void add(const _key_type& key, const _value_type& value) = 0;

	virtual void add(const keyValuePair& keyValue) = 0;

	virtual void remove(const _key_type& key) = 0;

	virtual void clear() = 0;

	virtual bool containsKey(const _key_type& key) const = 0;

	virtual bool containsValue(const _value_type& value) const = 0;

	virtual bool contains(const keyValuePair& keyValue) const = 0;

	virtual keyValuePair findKey(const _key_type& key) const = 0;

	virtual keyValuePair findValue(const _value_type& value) const = 0;

	virtual void set(const _key_type& key, const _value_type& value) = 0;

	virtual _value_type& get(const _key_type& key) = 0;

	virtual keyValuePair& get(int index) const = 0;
	
	virtual int count() const = 0;

	virtual bool isEmpty() const = 0;

	_value_type& operator[](const _key_type& key);
};

//implementation IDictionary::iterator

template<typename _key_type, typename _value_type>
IDictionary<_key_type, _value_type>::iterator::iterator(IDictionary* dictionary = NULL, int index = 0):
	mDictionary(dictionary), mIndex(index) 
{
}
	
template<typename _key_type, typename _value_type>
int IDictionary<_key_type, _value_type>::iterator::index() const
{
	return mIndex;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator+(int offs)
{
	return iterator(mArray, mIndex + offs);
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator-(int offs)
{
	return iterator(mArray, mIndex - offs);
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator& IDictionary<_key_type, _value_type>::iterator::operator++() // ++A;
{
	mIndex++;
	return *this ;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator++(int) // A++;
{
	iterator temp = *this;

	mIndex++;
	return temp ;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator& IDictionary<_key_type, _value_type>::iterator::operator--() // --A;
{
	mIndex--;
	return *this ;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator--(int) // A--;		
{
	iterator temp = *this;
	mIndex--;
	return temp ;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator+=(int offs)
{
	*this = *this + offs; 
	return *this;
}

template<typename _key_type, typename _value_type>
typename IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::iterator::operator-=(int offs)
{
	*this = *this - offs; 
	return *this;
}	

template<typename _key_type, typename _value_type>
bool IDictionary<_key_type, _value_type>::iterator::operator>(const iterator& itr)
{
	return mIndex > itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool IDictionary<_key_type, _value_type>::iterator::operator<(const iterator& itr)
{
	return mIndex < itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool IDictionary<_key_type, _value_type>::iterator::operator>=(const iterator& itr)
{
	return mIndex >= itr->mIndex;
}

template<typename _key_type, typename _value_type>
bool IDictionary<_key_type, _value_type>::iterator::operator<=(const iterator& itr)
{
	return mIndex <= itr->mIndex;
}

template<typename _key_type, typename _value_type>
IDictionary<_key_type, _value_type>::iterator::operator bool()
{
	return mIndex >= 0 && mIndex < mDictionary->count();
}

template<typename _key_type, typename _value_type>
_key_type& IDictionary<_key_type, _value_type>::iterator::key() const
{
	return mDictionary->get(mIndex).mFirst;
}

template<typename _key_type, typename _value_type>
_value_type& IDictionary<_key_type, _value_type>::iterator::value() const
{
	return mDictionary->get(mIndex).mSecond;
}

//implementation IDictionary
template<typename _key_type, typename _value_type>
virtual IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::begin()
{
	return iterator(this, 0);
}

template<typename _key_type, typename _value_type>
virtual IDictionary<_key_type, _value_type>::iterator IDictionary<_key_type, _value_type>::end()
{
	return iterator(this, count() - 1);
}

template<typename _key_type, typename _value_type>
_value_type& IDictionary<_key_type, _value_type>::operator[](const _key_type& key)
{
	return get(key);
}

#endif // IDICTIONARY_H
