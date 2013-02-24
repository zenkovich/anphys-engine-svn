#include "camera_2d.h"


REGIST_TYPE(grCamera2D)


grCamera2D::grCamera2D():grCamera(), mAngle(0), mScale(1.0f), mPosition(0.0f, 0.0f)
{
}

grCamera2D::grCamera2D( const vec2& screenSize, grRenderBase* render ):grCamera(screenSize, render), mAngle(0), mScale(1.0f), 
	mPosition(0.0f, 0.0f)
{
}

grCamera2D::grCamera2D( const vec2& screenSize, grRenderBase* render, const vec2& pos, const vec2& scale, float angle )
	:grCamera(screenSize, render), mPosition(pos), mScale(scale), mAngle(angle)
{
}

grCamera2D::~grCamera2D()
{
}

void grCamera2D::initialize( const vec2& screenSize, grRenderBase* render )
{
	grCamera::initialize(screenSize, render);
	mPosition = vec2(0, 0);
	mScale= vec2(1.0f);
	mAngle = 0;
}

void grCamera2D::initialize( const vec2& screenSize, grRenderBase* render, const vec2& pos, const vec2& scale, float angle )
{
	grCamera::initialize(screenSize, render);
	mPosition = pos;
	mScale= scale;
	mAngle = angle;
}
