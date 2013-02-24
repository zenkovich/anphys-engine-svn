#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "util/math/mmath.h"

#include "camera.h"

struct grCamera2D:public grCamera
{
	DEFINE_TYPE(grCamera2D)

	vec2  mPosition;
	vec2  mScale;
	float mAngle;

//functions
	grCamera2D();
	grCamera2D(const vec2& screenSize, grRenderBase* render);
	grCamera2D(const vec2& screenSize, grRenderBase* render, const vec2& pos, const vec2& scale, float angle);
	~grCamera2D(); 

	void initialize(const vec2& screenSize, grRenderBase* render);
	void initialize(const vec2& screenSize, grRenderBase* render, const vec2& pos, const vec2& scale, float angle);
};

#endif //CAMERA_2D_H