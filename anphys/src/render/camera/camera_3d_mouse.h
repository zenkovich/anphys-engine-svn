#ifndef CAMERA_3D_MOUSE_H
#define CAMERA_3D_MOUSE_H

#include "camera_3d.h"

struct grCamera3DMouse:public grCamera3D
{
	float mSpeed;
	float mShiftSpeed;

	float mXAngle;
	float mYAngle;

	float mXMinAngle;
	float mXMaxAngle;

	vec3  mDirection;

//functions
	grCamera3DMouse(const vec3& position = vec3(0, 0, 0), float xMinAngle = -80.0f, float xMaxAngle = 80.0f, float speed = 2.0f, float shiftSpeed = 30.0f);

	void mouseMoved(const vec2& diff);
	void move(const vec3& movingDirection,bool shifting);
	void moveForward(bool shifting);
	void moveBack(bool shifting);
	void moveLeft(bool shifting);
	void moveRight(bool shifting);
	void moveUp(bool shifting);
	void moveDown(bool shifting);

	void update(float dt);
};

#endif //CAMERA_3D_MOUSE_H