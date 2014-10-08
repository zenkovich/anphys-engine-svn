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

public:
	cLayout(const vec2f& LTRelative = vec2f(), const vec2f& LTAbsolute = vec2f(), 
		    const vec2f& RBRelative = vec2f(1, 1), const vec2f& RBAbsolute = vec2f(),
			const vec2f& minSize = vec2f(), const vec2f& maxSize = vec2f(FLT_MAX, FLT_MAX));

	void update(const vec2f& relPos, const vec2f& relSize, bool pixelPerfect = true);

	fRect getRect() const;

	cLayout& fixWidth(float width);
	cLayout& fixHeight(float height);
	cLayout& fixSize(const vec2f& size);

	cLayout& minWidth(float width);
	cLayout& minHeight(float height);
	cLayout& minSize(const vec2f& size);

	cLayout& maxWidth(float width);
	cLayout& maxHeight(float height);
	cLayout& maxSize(const vec2f& size);

	cLayout& absPosition(const vec2f& position);
	cLayout& leftTopAbs(const vec2f& position);
	cLayout& rightDownAbs(const vec2f& position);
	cLayout& leftAbs(float value);
	cLayout& rightAbs(float value);
	cLayout& topAbs(float value);
	cLayout& downAbs(float value);

	cLayout& leftTopRel(const vec2f& relPosition);
	cLayout& rightDownRel(const vec2f& relPosition);

	float left() const;
	float right() const;
	float top() const;
	float down() const;

	vec2f getltCorner() const;
	vec2f getrtCorner() const;
	vec2f getldCorner() const;
	vec2f getrdCorner() const;

	static cLayout both(const fRect& border = fRect());
	static cLayout relative(const vec2f& relSize, const vec2f& relPos = vec2f());
	static cLayout fixed(const vec2f& size, const vec2f& position = vec2f());
};

//implementation
cLayout::cLayout(const vec2f& LTRelative /*= vec2f()*/, const vec2f& LTAbsolute /*= vec2f()*/, 
		         const vec2f& RBRelative /*= vec2f(1, 1)*/, const vec2f& RBAbsolute /*= vec2f()*/,
		         const vec2f& minSize /*= vec2f()*/, const vec2f& maxSize /*= vec2f(FLT_MAX, FLT_MAX)*/):
	mLTRelative(LTRelative), mLTAbsolute(LTAbsolute), mRBRelative(RBRelative),
	mRBAbsolute(RBAbsolute), mMinSize(minSize), mMaxSize(maxSize)
{
	update(vec2f(), vec2f());
}

void cLayout::update(const vec2f& relPos, const vec2f& relSize, bool pixelPerfect /*= true*/) 
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

fRect cLayout::getRect() const 
{
	return fRect(mPosition, mPosition + mSize);
}

cLayout& cLayout::fixWidth(float width) 
{
	mRBRelative.x = mLTRelative.x;
	mRBAbsolute.x = mRBAbsolute.x + width;
	return *this;
}

cLayout& cLayout::fixHeight(float height) 
{
	mRBRelative.y = mLTRelative.y;
	mRBAbsolute.y = mRBAbsolute.y + height;
	return *this;
}

cLayout& cLayout::fixSize(const vec2f& size) 
{
	return fixHeight(size.y).fixWidth(size.x);
}

cLayout& cLayout::minWidth(float width) 
{
	mMinSize.x = width;
	return *this;
}

cLayout& cLayout::minHeight(float height) 
{
	mMinSize.y = height;
	return *this;
}

cLayout& cLayout::minSize(const vec2f& size) 
{
	mMinSize = size;
	return *this;
}

cLayout& cLayout::maxWidth(float width) 
{
	mMaxSize.x = width;
	return *this;
}

cLayout& cLayout::maxHeight(float height) 
{
	mMaxSize.y = height;
	return *this;
}

cLayout& cLayout::maxSize(const vec2f& size) 
{
	mMaxSize = size;
	return *this;
}

cLayout& cLayout::absPosition(const vec2f& position)
{
	vec2f dd = position - mLTAbsolute;
	mLTAbsolute += dd;
	mRBAbsolute += dd;
	return *this;
}

cLayout& cLayout::leftTopAbs(const vec2f& position)
{
	mLTAbsolute = position;
	return *this;
}

cLayout& cLayout::rightDownAbs(const vec2f& position)
{
	mRBAbsolute = position;
	return *this;
}

cLayout& cLayout::leftTopRel(const vec2f& relPosition)
{
	mLTRelative = relPosition;
	return *this;
}

cLayout& cLayout::rightDownRel(const vec2f& relPosition)
{
	mRBRelative = relPosition;
	return *this;
}

cLayout& cLayout::leftAbs(float value) 
{
	mLTAbsolute.x = value;
	return *this;
}

cLayout& cLayout::rightAbs(float value) 
{
	mRBAbsolute.x = value;
	return *this;
}

cLayout& cLayout::topAbs(float value) 
{
	mLTAbsolute.y = value;
	return *this;
}

cLayout& cLayout::downAbs(float value) 
{
	mRBAbsolute.y = value;
	return *this;
}

float cLayout::left() const 
{
	return mPosition.x;
}

float cLayout::right() const 
{
	return mPosition.x + mSize.x;
}

float cLayout::top() const 
{
	return mPosition.y;
}

float cLayout::down() const 
{
	return mPosition.y + mSize.y;
}

vec2f cLayout::getltCorner() const
{
	return mPosition; 
}

vec2f cLayout::getrtCorner() const
{ 
	return vec2f(mPosition.x + mSize.x, mPosition.y); 
}

vec2f cLayout::getldCorner() const
{ 
	return vec2f(mPosition.x, mPosition.y + mSize.y);
}

vec2f cLayout::getrdCorner() const
{ 
	return mPosition + mSize; 
}

cLayout cLayout::both(const fRect& border /*= fRect()*/)
{
	return cLayout(vec2f(), vec2f(border.left, border.top), vec2f(1, 1), vec2f(-border.right, -border.down));
}

cLayout cLayout::relative(const vec2f& relSize, const vec2f& relPos /*= vec2f()*/) 
{
	return cLayout(relPos, vec2f(), relPos + relSize, vec2f());
}

cLayout cLayout::fixed(const vec2f& size, const vec2f& position /*= vec2f()*/)
{
	return cLayout(vec2f(), position, vec2f(0, 0), size + position, size, size);
}

CLOSE_O2_NAMESPACE

#endif // LAYOUT_H
