#ifndef POOL_H
#define POOL_H

#include "assert.h"
#include "util/containers/array.h"

OPEN_O2_NAMESPACE

template<typename _type>
class pool
{
	array<_type*> mObjects;
	int           mChunkSize;

public:
	pool(int initialCount = 5, int chunkSize = 5);

	~pool();

	void setChunkSize(int chunkSize);

	int getChunkSize() const;

	_type* take();

	void free(_type* obj);

	void createObjects(int count);
};

//implementation
template<typename _type>
pool<_type>::pool(int initialCount = 5, int chunkSize = 5):
	mChunkSize(chunkSize)
{
	createObjects(initialCount);
}
	
template<typename _type>
pool<_type>::~pool()
{
	for (int i = 0; i < mObjects.count(); i++)
		safe_release(mObjects[i]);

	mObjects.clear();
}

template<typename _type>
void pool<_type>::setChunkSize(int chunkSize)
{
	mChunkSize = chunkSize;
}

template<typename _type>
int pool<_type>::getChunkSize() const
{
	return mChunkSize;
}

template<typename _type>
_type* pool<_type>::take()
{
	if (mObjects.count() == 0)
		createObjects(mChunkSize);

	return mObjects.popBack();
}

template<typename _type>
void pool<_type>::free(_type* obj)
{
	mObjects.add(obj);
}

template<typename _type>
void pool<_type>::createObjects(int count)
{
	for (int i = 0; i < count ; i++)
		mObjects.add(mnew _type());
}

CLOSE_O2_NAMESPACE

#endif // POOL_H
