#include "rects_packer.h"

#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

cRectsPacker::cRectsPacker(const vec2f& maxSize):mMaxSize(maxSize), mRectsPool(25, 25)
{
}

cRectsPacker::~cRectsPacker()
{
	clear();
}

cRectsPacker::rect* cRectsPacker::addRect(const vec2f& size)
{
	rect* newRect = mRectsPool.take();
	newRect->mSize = size;
	newRect->mRect = fRect();
	mRects.add(newRect);
	return newRect;
}

void cRectsPacker::removeRect(rect* remRect)
{
	mRects.remove(remRect);
	mRectsPool.free(remRect);
}

void cRectsPacker::clear()
{
	foreach(rect*, mRects, rt)
		mRectsPool.free(*rt);

	mRects.clear();
}

void cRectsPacker::setMaxSize(const vec2f& maxSize)
{
	mMaxSize = maxSize;
}

vec2f cRectsPacker::getMaxSize() const
{
	return mMaxSize;
}

bool cRectsPacker::pack()
{
	mRects.sort();

	return false;
}

CLOSE_O2_NAMESPACE