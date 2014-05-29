#ifndef RECT_DRAWABLE_H
#define RECT_DRAWABLE_H

#include "public.h"
#include "util/objects.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class IRectDrawable: public virtual IDrawable
{
protected:
	vec2f  mPosition;
	vec2f  mSize;
	vec2f  mPivot;
	float  mRotation;
	color4 mColor;

public:
	PROPERTY(IRectDrawable, vec2f)  position;
	PROPERTY(IRectDrawable, vec2f)  size;
	PROPERTY(IRectDrawable, vec2f)  pivot;
	PROPERTY(IRectDrawable, fRect)  rect;
	PROPERTY(IRectDrawable, float)  rotation;
	PROPERTY(IRectDrawable, float)  transparency;
	PROPERTY(IRectDrawable, color4) color;

	IRectDrawable(const vec2f& size = vec2f(), const vec2f& position = vec2f(), float rotation = 0.0f, 
		          const color4& color = color4::white(), const vec2f& pivot = vec2f());

	virtual ~IRectDrawable() {}

	virtual void draw() {}

	virtual void setPosition(const vec2f& position);
	virtual vec2f getPosition() const;

	virtual void setSize(const vec2f& size);
	virtual vec2f getSize() const;

	virtual void setPivot(const vec2f& pivot);
	virtual vec2f getPivot() const;

	virtual void setRect(const fRect& rect);
	virtual fRect getRect() const;

	virtual void setColor(const color4& color);
	virtual color4 getColor() const;

	virtual void setRotation(float rotation);
	virtual float getRotation() const;

	virtual void setTransparency(float transparency);
	virtual float getTransparency() const;

protected:
	virtual void positionChanged() {}
	virtual void sizeChanged() {}
	virtual void pivotChanged() {}
	virtual void rotationChanged() {}
	virtual void colorChanged() {}

	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //RECT_DRAWABLE_H
