#ifndef RECT_PACKER_H
#define RECT_PACKER_H

#include "public.h"
#include "util/pool.h"

OPEN_O2_NAMESPACE

class cRectsPacker
{
public:
	struct rect
	{
		fRect mRect;
		vec2f mSize;

		rect(const vec2f& size):mSize(size) {}
	};

protected:
	pool<rect>   mRectsPool;
	array<rect*> mRects;
	vec2f        mMaxSize;

public:
	cRectsPacker(const vec2f&  maxSize);
	~cRectsPacker();

	rect* addRect(const vec2f&  size);
	void removeRect(rect* remRect);
	void clear();

	void setMaxSize(const vec2f&  maxSize);
	vec2f getMaxSize() const;

	bool pack();
};

CLOSE_O2_NAMESPACE

#endif //RECT_PACKER_H