#ifndef CONTAINER_ARRAY_H
#define CONTAINER_ARRAY_H

#include "util/containers/enumerable.h"
#include "util/assert.h"

OPEN_O2_NAMESPACE

template<typename _type>
class array: public IEnumerable<_type>
{
	_type* mValues;
	int    mCount;
	int    mCapacity;

public:
	array(int capacity = 5);

	array(const array& arr);

	virtual ~array();

	array& operator=(const array& arr);

	bool operator==(const array& arr);

	bool operator!=(const array& arr);

	int count() const;

	bool isEmpty() const;

	int capacity() const;

	void resize(int newCount);

	void reserve(int newCapacity);

	_type& get(int idx);

	void set(int idx, const _type& value);

	_type& add(const _type& value);

	_type& popBack();

	_type& insert(const _type& value, int position);

	int find(const _type& value);

	bool contains(const _type& value);

	void remove(int idx);

	void removeRange(int begin, int end);

	bool remove(const _type& value);

	void clear();
	
	void sort(void (*compareFunc)(_type&, _type&));
	void sort();

protected:
	int getReservingSize(int size);
	void quickSort(void (*compareFunc)(_type&, _type&), int left, int right);

	template<typename _ct>
	static bool stdComparer(_ct& a, _ct& b) { return a < b; }
};

CLOSE_O2_NAMESPACE

#endif //CONTAINER_ARRAY_H
