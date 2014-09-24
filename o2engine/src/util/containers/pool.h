#ifndef POOL_H
#define POOL_H

#include "assert.h"
#include "array.h"

template<typename _type>
class pool
{
	array<_type*> mObjects;
	int           mChunkSize;

public:
	pool(int initialCount = 5, int chunkSize = 5):mChunkSize(chunkSize) 
	{
		createObjects(initialCount);
	}

	~pool()
	{
		for (int i = 0; i < mObjects.count(); i++)
			delete mObjects[i];
		mObjects.clear();
	}

	void setChunkSize(int chunkSize)
	{
		mChunkSize = chunkSize;
	}

	int getChunkSize() const 
	{
		return mChunkSize;
	}

	_type* take()
	{
		if (mObjects.count() == 0)
			createObjects(mChunkSize);

		return mObjects.popBack();
	}

	void free(_type* obj)
	{
		mObjects.add(obj);
	}

	void createObjects(int count) 
	{
		for (int i = 0; i < count ; i++)
			mObjects.add(new _type());
	}
};

#endif // POOL_H
