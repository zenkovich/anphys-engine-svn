#ifndef CAMERA_3D_MOUSE_H
#define CAMERA_3D_MOUSE_H

#include "util/math/mmath.h"
#include "camera.h"

struct grCamera3DMouse:public grCamera
{	
	DEFINE_TYPE(grCamera3DMouse)

	float  mAngleX;
	float  mAngleY;
	vec3   mPosition;
	vec3   mLookPoint;
	vec3   mSpeed;
	vec3   mOrientVec;
	mat3x3 mOrient;
	float  mSence;
	float  mSenceShift; 
	float  mSpeedFriction;
	float  mAxisSence;
	float  mMaxSpeed;
	float  mDt;

//functions
	grCamera3DMouse();
	grCamera3DMouse(const vec2& screenSize, grRenderBase* render);	
	grCamera3DMouse(const vec2& screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, 
		            float maxSpeed);	
	grCamera3DMouse(const vec2& screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence,
		            float maxSpeed, vec3 pos, float angleX = 0, float angleY = 0);

	void initialize(const vec2& screenSize, grRenderBase* render);
	void initialize(const vec2& screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, 
		            float maxSpeed);
	void initialize(const vec2& screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, 
		            float maxSpeed, vec3 pos, float angleX = 0, float angleY = 0);

	void mouseMove(const vec2& diff);
	void moveForward(bool shift);
	void moveBack(bool shift);
	void moveLeft(bool shift);
	void moveRight(bool shift);
	void moveUp(bool shift);
	void moveDown(bool shift);
	void moveVec(vec3 vec);

	void specRenderUpdate(float dt);

	void update(float dt);	
};

#endif //CAMERA_3D_MOUSE_H
