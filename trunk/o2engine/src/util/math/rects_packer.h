#ifndef RECT_PACKER_H
#define RECT_PACKER_H

#include "public.h"
#include "util/containers/pool.h"
#include "util/containers/array.h"
#include "util/containers/tree_interface.h"

OPEN_O2_NAMESPACE

class cRectsPacker
{
public:
	struct rect
	{
		fRect mRect;
		vec2f mSize;

		rect(const vec2f& size = vec2f()):mSize(size) {}
	};

protected:
	struct quadNode: public ITreeNode<quadNode>
	{
		fRect mRect;
	};

	pool<rect>   mRectsPool;
	array<rect*> mRects;
	vec2f        mMaxSize;

public:
	cRectsPacker(const vec2f&  maxSize = vec2f(512, 512));
	~cRectsPacker();

	rect* addRect(const vec2f&  size);
	void removeRect(rect* remRect);
	void clear();

	void setMaxSize(const vec2f&  maxSize);
	vec2f getMaxSize() const;

	bool pack();

protected:
	static bool rectSizeCompare(rect*& a, rect*& b);
};

CLOSE_O2_NAMESPACE

#endif //RECT_PACKER_H