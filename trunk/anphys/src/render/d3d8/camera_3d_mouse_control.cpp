#include "../../engine/engine_options.h"

#ifdef RENDER_D3D8
#include "camera_3d_mouse_control.h"


grCameraD3DMouseControl::grCameraD3DMouseControl():grCameraD3D(vec2(0), NULL) {}

grCameraD3DMouseControl::grCameraD3DMouseControl(vec2 screenSize, grRenderBase* render)
{
	initialize(screenSize, render);
}

grCameraD3DMouseControl::grCameraD3DMouseControl(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed)
{
	initialize(screenSize, render, sence, shiftSence, speedFriction, axisSence, maxSpeed);
}
	
grCameraD3DMouseControl::grCameraD3DMouseControl(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		                                         vec3 pos, float angleX, float angleY)
{
	initialize(screenSize, render, sence, shiftSence, speedFriction, axisSence, maxSpeed, pos, angleX, angleY);
}

void grCameraD3DMouseControl::initialize(vec2 screenSize, grRenderBase* render)
{
	grCameraD3D::initialize(screenSize, render);

	mPosition = vec3(0);
	mOrient = nullMatr();
	mAngleX = mAngleY = 0;

	mSence = 10;
	mSenceShift = 100;
	mSpeed = 0;
	mSpeedFriction = 10;
	mAxisSence = 0.1f;
	mMaxSpeed = 10;
	mDt = 0;

	update(0);
}

void grCameraD3DMouseControl::initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed)
{
	grCameraD3D::initialize(screenSize, render);

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

void grCameraD3DMouseControl::initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		            vec3 pos, float angleX, float angleY)
{
	grCameraD3D::initialize(screenSize, render);

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

void grCameraD3DMouseControl::mouseMove(vec2 diff)
{
	mAngleX += diff.y*mAxisSence;
	mAngleY += diff.x*mAxisSence;
}

void grCameraD3DMouseControl::moveForward(bool shift)
{
	mSpeed += vec3(0,0,1)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveBack(bool shift)
{
	mSpeed += vec3(0,0,-1)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveLeft(bool shift)
{
	mSpeed += vec3(-1,0,0)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveRight(bool shift)
{
	mSpeed += vec3(1,0,0)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveUp(bool shift)
{
	mSpeed += vec3(0,1,0)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveDown(bool shift)
{
	mSpeed += vec3(0,-1,0)*((shift) ? mSenceShift:mSence)*mDt;
}

void grCameraD3DMouseControl::moveVec(vec3 vec)
{
	mPosition += vec;
}

void grCameraD3DMouseControl::update(float dt)
{
	mDt = dt;

	if (mSpeed.len() > mMaxSpeed) mSpeed = mSpeed.normalize()*mMaxSpeed;

	mPosition += mSpeed*dt;

	float speed = mSpeed.len();
	mSpeed = mSpeed/speed*max(speed - mSpeedFriction*dt, 0);

	mOrient.Identity();
	mat3x3 mx,my;
	my.SetRotationY(mAngleY);
	mx.SetRotationX(mAngleX);

	mOrient=mx*my;

	mOrientVec=vec3(0,0,1)*mOrient;
	mLookPoint = mPosition + mOrientVec;

	D3DXMatrixLookAtLH(&mViewMatrix, &D3DXVECTOR3(mPosition.x,mPosition.y,mPosition.z), &D3DXVECTOR3(mLookPoint.x,mLookPoint.y,mLookPoint.z), &D3DXVECTOR3(0,1,0));
	D3DXMatrixRotationX(&mWorldMatrix, 0);
	D3DXMatrixPerspectiveFovLH(&mProjectMatrix, D3DX_PI/4.0f, (float)mScreenSize.y/(float)mScreenSize.x, 0.1f, 1000000.0f);

	grCameraD3D::update(dt);
}

#endif //RENDER_D3D8
	