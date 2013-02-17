#ifndef CAMERA_3D_MOUSE_H
#define CAMERA_3D_MOUSE_H

#include "util/math/mmath.h"
#include "camera.h"

struct grCamera3DMouse:public grCamera
{	
	float mAngleX, 
		  mAngleY;
	vec3  mPosition, 
		  mLookPoint, 
		  mSpeed,
		  mOrientVec;
	mat3x3 mOrient;
	float mSence, 
		  mSenceShift, 
		  mSpeedFriction, 
		  mAxisSence,
		  mMaxSpeed;
	float mDt;

	grCamera3DMouse();
	grCamera3DMouse(vec2 screenSize, grRenderBase* render);	
	grCamera3DMouse(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed);	
	grCamera3DMouse(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		                    vec3 pos, float angleX = 0, float angleY = 0);

	void initialize(vec2 screenSize, grRenderBase* render);
	void initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed);
	void initialize(vec2 screenSize, grRenderBase* render, float sence, float shiftSence, float speedFriction, float axisSence, float maxSpeed,
		            vec3 pos, float angleX = 0, float angleY = 0);

	void mouseMove(vec2 diff);
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
