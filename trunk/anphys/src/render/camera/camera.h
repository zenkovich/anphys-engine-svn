#ifndef CAMERA_H
#define CAMERA_H

#include "util/math/mmath.h"
#include "util/other/type_intexation.h"

struct grRenderBase;

struct grCamera
{
	DEFINE_TYPE(grCamera)

	vec2          mScreenSize;
	grRenderBase* mRenderBase;

//functions
	grCamera();
	grCamera(const vec2& screenSize, grRenderBase* render);
	virtual ~grCamera(); 

	virtual void initialize(const vec2& screenSize, grRenderBase* render) ;

	virtual void specRenderUpdate(float dt) {}
	virtual void update(float dt) {}
};

#endif //CAMERA_H