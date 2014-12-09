#ifndef CAMERA_H
#define CAMERA_H

#include "public.h"

OPEN_O2_NAMESPACE

/** Camera. Contains pivot, position, scale and rotations. Works as sprite. */
class grCamera
{
public:
	vec2f mPivot;    /**< Reative pivot. (0...1). */
	vec2f mPosition; /**< Position of pivot camera. */
	vec2f mScale;    /**< Scale of camera, around pivot. */
	float mRotation; /**< Rotation of camera, around pivot. */

	/** ctor. */
	grCamera(const vec2f& pos = vec2f(), const vec2f& scale = vec2f(1, 1), float rotation = 0, 
		     const vec2f& pivot = vec2f())
	{
		mPivot = pivot;
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets the parametres of camera. */
	void set(const vec2f& pos = vec2f(), const vec2f& scale = vec2f(1, 1), float rotation = 0, 
		     const vec2f& pivot = vec2f())
	{
		mPivot = pivot;
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets parametres from basis. Shear not supporting. */
	void set(const basis& bas)
	{
		mPivot = vec2f();
		mPosition = bas.offs;
		mScale = bas.getScale();
		mRotation = bas.getAngle();
	}
};

CLOSE_O2_NAMESPACE

#endif //CAMERA_H