#include "camera.h"

REGIST_TYPE(grCamera)

grCamera::grCamera():mRenderBase(NULL) {}

grCamera::grCamera(const vec2& screenSize, grRenderBase* render)
{
	initialize(screenSize, render); 
}

grCamera::~grCamera() {} 

void grCamera::initialize(const vec2& screenSize, grRenderBase* render) 
{ 
	mScreenSize = screenSize; 
	mRenderBase = render; 
}