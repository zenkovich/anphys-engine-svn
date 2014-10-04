#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "util/containers/idictionary.h"
#include "util/assert.h"
#include "util/mem_utils/memory_manager.h"
#include "util/mem_utils/alloc_operators.h"
#include "util/containers/array.h"
#include "util/containers/pair.h"

OPEN_O2_NAMESPACE

template<typename _key_type, typename _value_type>
class dict: public IDictionary<_key_type, _value_type>
{
public:
	typedef pair< _key_type, _value_type > keyValuePair;

protected:
	IArray< keyValuePair >* mPairs;

public:
	dict();

	dict(const dict& other);

	~dict();

	bool operator==(const dict& other);

	bool operator!=(const dict& other);

	dict& operator=(const dict& other);

	template<typename _type>
	void setupContainerType()
	{
		IArray* tmp = mPairs->clone();
		safe_release(mPairs);
		mPairs = mnew _type(tmp);
		safe_release(tmp);
	}

	void add(const _key_type& key, const _value_type& value);

	void add(const keyValuePair& keyValue);

	void remove(const _key_type& key);

	void clear();

	bool containsKey(const _key_type& key) const;

	bool containsValue(const _value_type& value) const;

	bool contains(const keyValuePair& keyValue) const;

	keyValuePair findKey(const _key_type& key) const;

	keyValuePair findValue(const _value_type& value) const;

	void set(const _key_type& key, const _value_type& value);

	_value_type& get(const _key_type& key);

	keyValuePair& get(int index) const;
	
	int count() const;

	bool isEmpty() const;
	
	void sort(bool (*compareFunc)(keyValuePair&, keyValuePair&));

	void sort();
};

//implementation
template<typename _key_type, typename _value_type>
dict<_key_type, _value_type>::dict()
{
	mPairs = mnew array< keyValuePair >();
}

template<typename _key_type, typename _value_type>
dict<_key_type, _value_type>::dict(const dict<_key_type, _value_type>& other)
{
	mPairs = other.mPairs->clone();
}

template<typename _key_type, typename _value_type>
dict<_key_type, _value_type>::~dict()
{
	safe_release(mPairs);
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::operator==(const dict& other)
{
	if (count() != other.count())
		return false;

	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (*kv != other.mPairs[kv.index()])
			return false;

	return true;
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::operator!=(const dict& other)
{
	if (count() == other.count())
		return false;

	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (*kv != other.mPairs[kv.index()])
			return true;

	return false;
}

template<typename _key_type, typename _value_type>
dict& dict<_key_type, _value_type>::operator=(const dict& other)
{
	safe_release(mPairs);
	mPairs = other.mPairs->clone();
	return *this;
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::add(const _key_type& key, const _value_type& value)
{
	mPairs->add(keyValuePair(key, value));
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::add(const keyValuePair& keyValue)
{
	mPairs->add(keyValue);
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::remove(const _key_type& key)
{
	int idx = -1;
	foreach(IArray< keyValuePair >, *mPairs, kv)
	{
		if (kv->mFirst == key)
		{
			idx = kv.index();
			break;
		}
	}

	if (idx >= 0)
		mPairs->remove(idx);
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::clear()
{
	mPairs->clear();
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::containsKey(const _key_type& key) const
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (kv->mFirst == key)
			return true;

	return false;
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::containsValue(const _value_type& value) const
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (kv->mSecond == value)
			return true;

	return false;
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::contains(const keyValuePair& keyValue) const
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (*kv == keyValue)
			return true;

	return false;
}

template<typename _key_type, typename _value_type>
keyValuePair dict<_key_type, _value_type>::findKey(const _key_type& key) const
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (kv->mFirst == key)
			return *kv;

	return keyValuePair();
}

template<typename _key_type, typename _value_type>
keyValuePair dict<_key_type, _value_type>::findValue(const _value_type& value) const
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (kv->mSecond == value)
			return *kv;

	return keyValuePair();
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::set(const _key_type& key, const _value_type& value)
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
	{
		if (kv->mFirst == key)
		{
			kv->mSecond = value;
			break;
		}
	}
}

template<typename _key_type, typename _value_type>
_value_type& dict<_key_type, _value_type>::get(const _key_type& key)
{
	foreach(IArray< keyValuePair >, *mPairs, kv)
		if (kv->mFirst == key)
			return kv->mSecond;

	assert(false, "Failed to get value from dictionary: not found key");

	return mPairs->get(0).mSecond;
}

template<typename _key_type, typename _value_type>
keyValuePair& dict<_key_type, _value_type>::get(int index) const
{
	return mPairs->get(index);
}
	
template<typename _key_type, typename _value_type>
int dict<_key_type, _value_type>::count() const
{
	return mPairs->count();
}

template<typename _key_type, typename _value_type>
bool dict<_key_type, _value_type>::isEmpty() const
{
	return count() == 0;
}
	
template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::sort(bool (*compareFunc)(keyValuePair&, keyValuePair&))
{
	mPairs->sort(compareFunc);
}

template<typename _key_type, typename _value_type>
void dict<_key_type, _value_type>::sort()
{
	mPairs->sort();
}

CLOSE_O2_NAMESPACE

#endif // DICTIONARY_H
