#include "camera_3d_mouse.h"


grCamera3DMouse::grCamera3DMouse( const vec3& position /*= vec3(0, 0, 0)*/, float xMinAngle /*= -80.0f*/, float xMaxAngle /*= 80.0f*/, 
	                              float speed /*= 20.0f*/, float shiftSpeed /*= 60.0f*/ ) :
	grCamera3D(position), mXAngle(0), mYAngle(0), mXMinAngle(xMinAngle), mXMaxAngle(xMaxAngle), mSpeed(speed), mShiftSpeed(shiftSpeed)
{
	update(0.0f);
}

void grCamera3DMouse::mouseMoved( const vec2& diff )
{
	float xSense = 1.0f, ySense = 1.0f;

	mXAngle = fclamp( mXAngle + diff.y*xSense, mXMinAngle, mXMaxAngle );
	mYAngle += diff.x*ySense;
}

void grCamera3DMouse::move( const vec3& movingDirection, bool shifting )
{
	mPosition += movingDirection*( shifting ? mShiftSpeed:mSpeed );
}

void grCamera3DMouse::update( float dt )
{
	mDirection = vec3(0, 0, 1);
	mDirection.RotateXYZ(rad(mXAngle), rad(mYAngle), 0);

	mLookPoint = mPosition + mDirection;
}

void grCamera3DMouse::moveForward( bool shifting )
{
	move(mDirection, shifting);
}

void grCamera3DMouse::moveBack( bool shifting )
{
	move(mDirection*-1.0f, shifting);
}

void grCamera3DMouse::moveLeft( bool shifting )
{
	move(mDirection^vec3(0, 1, 0), shifting);
}

void grCamera3DMouse::moveRight( bool shifting )
{
	move(mDirection^vec3(0, -1, 0), shifting);
}

void grCamera3DMouse::moveUp( bool shifting )
{
	move(vec3(0, 1, 0), shifting);
}

void grCamera3DMouse::moveDown( bool shifting )
{
	move(vec3(0, -1, 0), shifting);
}
