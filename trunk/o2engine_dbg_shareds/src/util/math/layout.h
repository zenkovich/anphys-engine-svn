#ifndef LAYOUT_H
#define LAYOUT_H

#include "util/public_namespace.h"
#include "util/math/vector2.h"
#include "util/smart_ptrs.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

struct cLayout
{
	vec2f mLTRelative;
	vec2f mLTAbsolute;
	vec2f mRBRelative;
	vec2f mRBAbsolute;
	vec2f mPosition;
	vec2f mSize;
	vec2f mMinSize;
	vec2f mMaxSize;

	cLayout(const vec2f& LTRelative = vec2f(), const vec2f& LTAbsolute = vec2f(), 
		    const vec2f& RBRelative = vec2f(1, 1), const vec2f& RBAbsolute = vec2f(),
			const vec2f& minSize = vec2f(), const vec2f& maxSize = vec2f(FLT_MAX, FLT_MAX)):
		mLTRelative(LTRelative), mLTAbsolute(LTAbsolute), mRBRelative(RBRelative),
		mRBAbsolute(RBAbsolute), mMinSize(minSize), mMaxSize(maxSize)
	{
		update(vec2f(), vec2f());
	}

	void update(const vec2f& relPos, const vec2f& relSize) 
	{
		mPosition = relSize.scale(mLTRelative) + mLTAbsolute + relPos;
		mSize = relSize.scale(mRBRelative) + mRBAbsolute + relPos - mPosition;		
		mSize.x = clamp(mSize.x, mMinSize.x, mMaxSize.x);
		mSize.y = clamp(mSize.y, mMinSize.y, mMaxSize.y);
	}

	fRect getRect() const 
	{
		return fRect(mPosition, mPosition + mSize);
	}

	static cLayout both(const fRect& border = fRect())
	{
		return cLayout(vec2f(), vec2f(border.left, border.top), vec2f(1, 1), vec2f(-border.right, -border.down));
	}

	static cLayout fixedSize(const vec2f& size, const vec2f& position)
	{
		return cLayout(vec2f(), position, vec2f(0, 0), size + position, size, size);
	}
};

CLOSE_O2_NAMESPACE

#endif // LAYOUT_H
