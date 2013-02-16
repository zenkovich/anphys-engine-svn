#ifndef CAMERA_H
#define CAMERA_H

//#define camTypeMouseControl 0;
#include "../util/math/mmath.h"

struct grRenderBase;

struct grCamera
{
	int mType;
	vec2 mScreenSize;
	grRenderBase* mRenderBase;

	grCamera() { mRenderBase = NULL; }
	grCamera(vec2 screenSize, grRenderBase* render) { initialize(screenSize, render); }
	virtual ~grCamera() {} 

	virtual void initialize(vec2 screenSize, grRenderBase* render) 
	{ 
		mScreenSize = screenSize; 
		mRenderBase = render; 
	}

	virtual void specRenderUpdate(float dt) {}
	virtual void update(float dt) {}
};

#endif //CAMERA_H