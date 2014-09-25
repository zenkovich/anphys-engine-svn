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

CLOSE_O2_NAMESPACE

#endif // POOL_H
