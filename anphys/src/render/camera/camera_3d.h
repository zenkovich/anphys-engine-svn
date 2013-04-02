#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include "util/math/mmath.h"

struct grCamera3D
{
	vec3  mPosition;
	vec3  mLookPoint;
	float mAngle;
	float mFov;

//functions
	grCamera3D(const vec3& position = vec3(0, 0, 0), const vec3& lookPoint = vec3(0, 0, 1), float angle = 0, 
		       float fov = Pi/4.0f):mPosition(position), mLookPoint(lookPoint), mAngle(angle), mFov(fov) {}
	virtual ~grCamera3D() {}
};

#endif //CAMERA_3D_H