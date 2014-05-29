#include "rect_drawable.h"

OPEN_O2_NAMESPACE

IRectDrawable::IRectDrawable(const vec2f& size /*= vec2f()*/, const vec2f& position /*= vec2f()*/, 
                             const color4& color /*= color4::white()*/, const vec2f& pivot /*= vec2f()*/):
	mPivot(pivot), mPosition(position), mSize(size), mColor(color)
{
	initializeProperties();
}

IRectDrawable::IRectDrawable( const IRectDrawable& drawable )
{
	mPosition = drawable.mPosition;
	mPivot = drawable.mPivot;
	mSize = drawable.mSize;
	mColor = drawable.mColor;

	initializeProperties();
}

void IRectDrawable::setPosition(const vec2f& position)
{
	if (equals(position, mPosition))
		return;

	mPosition = position;
	positionChanged();
}

vec2f IRectDrawable::getPosition() const
{
	return mPosition;
}

void IRectDrawable::setSize(const vec2f& size)
{
	if (equals(size, mSize))
		return;

	mSize= size;
	sizeChanged();
}

vec2f IRectDrawable::getSize() const
{
	return mSize;
}

void IRectDrawable::setPivot(const vec2f& pivot)
{
	if (equals(pivot, mPivot))
		return;

	mPivot = pivot;
	pivotChanged();
}

vec2f IRectDrawable::getPivot() const
{
	return mPivot;
}

void IRectDrawable::setRect(const fRect& rect)
{
	vec2f lt = mPosition - mPivot;
	if (equals(lt, rect.getltCorner()) && equals(lt + mSize, rect.getrdCorner()))
		return;

	mSize = rect.getSize();
	mPosition = rect.getltCorner() + mPivot;

	positionChanged();
	sizeChanged();
}

fRect IRectDrawable::getRect() const
{
	vec2f lt = mPosition - mPivot;
	return fRect(lt, lt + mSize);
}

void IRectDrawable::setColor(const color4& color)
{
	if (equals(color, mColor))
		return;

	mColor = color;
	colorChanged();
}

color4 IRectDrawable::getColor() const
{
	return mColor;
}

void IRectDrawable::setTransparency(float transparency)
{
	if (equals(mColor.af(), transparency))
		return;

	mColor.a = (int)(255.0f*transparency);
	colorChanged();
}

float IRectDrawable::getTransparency() const
{
	return mColor.af();
}

void IRectDrawable::setRelativePivot( const vec2f& relPivot )
{
	setPivot(relPivot.scale(mSize));
}

vec2f IRectDrawable::getRelativePivot() const
{
	return mPivot.invScale(mSize);
}

void IRectDrawable::initializeProperties()
{
	position.init(this, &IRectDrawable::setPosition, &IRectDrawable::getPosition);
	size.init(this, &IRectDrawable::setSize, &IRectDrawable::getSize);
	pivot.init(this, &IRectDrawable::setPivot, &IRectDrawable::getPivot);
	relPivot.init(this, &IRectDrawable::setRelativePivot, &IRectDrawable::getRelativePivot);
	rect.init(this, &IRectDrawable::setRect, &IRectDrawable::getRect);
	transparency.initNonConstSetter(this, &IRectDrawable::setTransparency, &IRectDrawable::getTransparency);
	color.init(this, &IRectDrawable::setColor, &IRectDrawable::getColor);
}

CLOSE_O2_NAMESPACE