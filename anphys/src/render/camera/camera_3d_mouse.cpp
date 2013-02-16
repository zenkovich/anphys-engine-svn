#include "camera_3d_mouse.h"

#include "render.h"


grCamera3DMouse::grCamera3DMouse():grCamera(vec2(0), NULL) {}

grCamera3DMouse::grCamera3DMouse(vec2 screenSize, grRenderBase* render)
{
	initialize(screenSize, render);
}

grCamera3DMouse::grCamera3DMouse(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed)
{
	initialize(screenSize, render, sence, shiftSence, speedFriction, axisSence, maxSpeed);
}
	
grCamera3DMouse::grCamera3DMouse(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		                                         vec3 pos, float angleX, float angleY)
{
	initialize(screenSize, render, sence, shiftSence, speedFriction, axisSence, maxSpeed, pos, angleX, angleY);
}

void grCamera3DMouse::initialize(vec2 screenSize, grRenderBase* render)
{
	grCamera::initialize(screenSize, render);

	mPosition = vec3(0);
	mOrient = nullMatr();
	mAngleX = mAngleY = 0;

	mSence = 30;
	mSenceShift = 100;
	mSpeed = 0;
	mSpeedFriction = 20;
	mAxisSence = 0.2f;
	mMaxSpeed = 20;
	mDt = 0;

	update(0);
}

void grCamera3DMouse::initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed)
{
	grCamera::initialize(screenSize, render);

	mPosition = vec3(0);
	mOrient = nullMatr();
	mAngleX = mAngleY = 0;

	mSence = sence;
	mSenceShift = shiftSence;
	mSpeed = 0;
	mSpeedFriction = speedFriction;
	mAxisSence = axisSence;
	mDt = 0;

	update(0);
}

void grCamera3DMouse::initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		            vec3 pos, float angleX, float angleY)
{
	grCamera::initialize(screenSize, render);

	mPosition = pos;
	mOrient = nullMatr();
	mAngleX = angleX;
	mAngleY = angleY;

	mSence = sence;
	mSenceShift = shiftSence;
	mSpeed = 0;
	mSpeedFriction = speedFriction;
	mAxisSence = axisSence;
	mDt = 0;

	update(0);
}

void grCamera3DMouse::mouseMove(vec2 diff)
{
	mAngleX += diff.y*mAxisSence;
	mAngleY += diff.x*mAxisSence;
}

void grCamera3DMouse::moveForward(bool shift)
{
	mSpeed += vec3(0,0,1)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveBack(bool shift)
{
	mSpeed += vec3(0,0,-1)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveLeft(bool shift)
{
	mSpeed += vec3(-1,0,0)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveRight(bool shift)
{
	mSpeed += vec3(1,0,0)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveUp(bool shift)
{
	mSpeed += vec3(0,1,0)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveDown(bool shift)
{
	mSpeed += vec3(0,-1,0)*((shift) ? mSenceShift:mSence)*mDt*mOrient;
}

void grCamera3DMouse::moveVec(vec3 vec)
{
	mPosition += vec;
}

void grCamera3DMouse::update(float dt)
{
	mDt = dt;

	if (mSpeed.len() > mMaxSpeed) mSpeed = mSpeed.normalize()*mMaxSpeed;

	mPosition += mSpeed*dt;

	float speed = mSpeed.len();
	mSpeed = mSpeed/speed*max(speed - mSpeedFriction*dt, 0);
	
	if (mAngleX > 75.0f) mAngleX = 75.0f;
	if (mAngleX < -75.0f) mAngleX = -75.0f;

	mOrient.Identity();
	mat3x3 mx,my;
	my.SetRotationY(rad(mAngleY));
	mx.SetRotationX(rad(mAngleX));

	mOrient = mx*my;

	mOrientVec = vec3(0,0,1)*mOrient;
	mLookPoint = mPosition + mOrientVec;

	specRenderUpdate(dt);
	
	grCamera::update(dt);
}