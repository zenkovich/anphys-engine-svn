#ifndef CAMERA_H
#define CAMERA_H

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE

class grCamera
{
public:
	vec2f mPosition;
	vec2f mScale;
	float mRotation;

	grCamera(const vec2f& pos = vec2f(0, 0), const vec2f& scale = vec2f(1, 1), float rotation = 0)
	{
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	void set(const vec2f& pos = vec2f(0, 0), const vec2f& scale = vec2f(1, 1), float rotation = 0)
	{
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	void set(const basis& bas)
	{
		mPosition = bas.offs;
		mScale = bas.getScale();
		mRotation = bas.getAngle();
	}
};

CLOSE_O2_NAMESPACE

#endif //CAMERA_H