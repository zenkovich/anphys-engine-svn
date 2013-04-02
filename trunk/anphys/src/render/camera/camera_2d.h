#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "util/math/mmath.h"

struct grCamera2D
{
	vec2  mPosition;
	vec2  mScale;
	float mAngle;

//functions
	grCamera2D(const vec2& position = vec2(0, 0), const vec2& scale = vec2(1, 1), float angle = 0):
		mPosition(position), mScale(scale), mAngle(angle) {}
};

#endif //CAMERA_2D_H