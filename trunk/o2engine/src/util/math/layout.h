#ifndef LAYOUT_H
#define LAYOUT_H

#include "util/public_namespace.h"
#include "util/math/vector2.h"
#include "util/smart_ptrs.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

struct layout
{
	vec2f mLTRelative;
	vec2f mLTAbsolute;
	vec2f mRBRelative;
	vec2f mRBAbsolute;
	vec2f mPosition;
	vec2f mSize;
	vec2f mMinSize;
	vec2f mMaxSize;

	layout(const vec2f& LTRelative = vec2f(), const vec2f& LTAbsolute = vec2f(), 
		    const vec2f& RBRelative = vec2f(1, 1), const vec2f& RBAbsolute = vec2f(),
			const vec2f& minSize = vec2f(), const vec2f& maxSize = vec2f(FLT_MAX, FLT_MAX)):
		mLTRelative(LTRelative), mLTAbsolute(LTAbsolute), mRBRelative(RBRelative),
		mRBAbsolute(RBAbsolute), mMinSize(minSize), mMaxSize(maxSize)
	{
		update(vec2f(), vec2f());
	}

	void update(const vec2f& relPos, const vec2f& relSize, bool pixelPerfect = true) 
	{
		mPosition = relSize.scale(mLTRelative) + mLTAbsolute + relPos;
		mSize = relSize.scale(mRBRelative) + mRBAbsolute + relPos - mPosition;		
		mSize.x = clamp(mSize.x, mMinSize.x, mMaxSize.x);
		mSize.y = clamp(mSize.y, mMinSize.y, mMaxSize.y);

		if (pixelPerfect)
		{
			mPosition.x = ceil(mPosition.x); mPosition.y = ceil(mPosition.y);
			mSize.x = ceil(mSize.x); mSize.y = ceil(mSize.y);
		}
	}

	fRect getRect() const 
	{
		return fRect(mPosition, mPosition + mSize);
	}

	layout& fixWidth(float width) 
	{
		mRBRelative.x = mLTRelative.x;
		mRBAbsolute.x = mRBAbsolute.x + width;
		return *this;
	}

	layout& fixHeight(float height) 
	{
		mRBRelative.y = mLTRelative.y;
		mRBAbsolute.y = mRBAbsolute.y + height;
		return *this;
	}

	layout& fixSize(const vec2f& size) 
	{
		return fixHeight(size.y).fixWidth(size.x);
	}

	layout& minWidth(float width) 
	{
		mMinSize.x = width;
		return *this;
	}

	layout& minHeight(float height) 
	{
		mMinSize.y = height;
		return *this;
	}

	layout& minSize(const vec2f& size) 
	{
		mMinSize = size;
		return *this;
	}

	layout& maxWidth(float width) 
	{
		mMaxSize.x = width;
		return *this;
	}

	layout& maxHeight(float height) 
	{
		mMaxSize.y = height;
		return *this;
	}

	layout& maxSize(const vec2f& size) 
	{
		mMaxSize = size;
		return *this;
	}

	layout& absPosition(const vec2f& position)
	{
		vec2f dd = position - mLTAbsolute;
		mLTAbsolute += dd;
		mRBAbsolute += dd;
		return *this;
	}

	layout& leftTopAbs(const vec2f& position)
	{
		mLTAbsolute = position;
		return *this;
	}

	layout& rightDownAbs(const vec2f& position)
	{
		mRBAbsolute = position;
		return *this;
	}

	layout& leftTopRel(const vec2f& relPosition)
	{
		mLTRelative = relPosition;
		return *this;
	}

	layout& rightDownRel(const vec2f& relPosition)
	{
		mRBRelative = relPosition;
		return *this;
	}

	layout& leftAbs(float value) 
	{
		mLTAbsolute.x = value;
		return *this;
	}

	layout& rightAbs(float value) 
	{
		mRBAbsolute.x = value;
		return *this;
	}

	layout& topAbs(float value) 
	{
		mLTAbsolute.y = value;
		return *this;
	}

	layout& downAbs(float value) 
	{
		mRBAbsolute.y = value;
		return *this;
	}

	float left() const 
	{
		return mPosition.x;
	}

	float right() const 
	{
		return mPosition.x + mSize.x;
	}

	float top() const 
	{
		return mPosition.y;
	}

	float down() const 
	{
		return mPosition.y + mSize.y;
	}

	vec2f getltCorner() const
	{
		return mPosition; 
	}

	vec2f getrtCorner() const
	{ 
		return vec2f(mPosition.x + mSize.x, mPosition.y); 
	}

	vec2f getldCorner() const
	{ 
		return vec2f(mPosition.x, mPosition.y + mSize.y);
	}

	vec2f getrdCorner() const
	{ 
		return mPosition + mSize; 
	}

	static layout both(const fRect& border = fRect())
	{
		return layout(vec2f(), vec2f(border.left, border.top), vec2f(1, 1), vec2f(-border.right, -border.down));
	}

	static layout relative(const vec2f& relSize, const vec2f& relPos = vec2f()) 
	{
		return layout(relPos, vec2f(), relPos + relSize, vec2f());
	}

	static layout fixed(const vec2f& size, const vec2f& position = vec2f())
	{
		return layout(vec2f(), position, vec2f(0, 0), size + position, size, size);
	}
};

CLOSE_O2_NAMESPACE

#endif // LAYOUT_H
