#include "pool.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
void pool<_type>::createObjects(int count)
{
	for (int i = 0; i < count ; i++)
		mObjects.add(new _type());
}

template<typename _type>
void pool<_type>::free(_type* obj)
{
	mObjects.add(obj);
}

template<typename _type>
_type* pool<_type>::take()
{
	if (mObjects.count() == 0)
		createObjects(mChunkSize);

	return mObjects.popBack();
}

template<typename _type>
int pool<_type>::getChunkSize() const
{
	return mChunkSize;
}

template<typename _type>
void pool<_type>::setChunkSize(int chunkSize)
{
	mChunkSize = chunkSize;
}

template<typename _type>
pool<_type>::~pool()
{
	for (int i = 0; i < mObjects.count(); i++)
		delete mObjects[i];
	mObjects.clear();
}

template<typename _type>
pool<_type>::pool(int initialCount /*= 5*/, int chunkSize /*= 5*/) :mChunkSize(chunkSize)
{
	createObjects(initialCount);
}

CLOSE_O2_NAMESPACE