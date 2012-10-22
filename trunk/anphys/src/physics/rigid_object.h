#ifndef RIGID_OBJECT_H
#define RIGID_OBJECT_H

#include "physics_object.h"


struct phRigidObject:public phObject
{
	/* from phObject
	{
		int mType;

		phScene* mScene;

		phObject():mScene(NULL) {}
		phObject(phScene* scene):mScene(scene) {}

		virtual void update(float dt) = 0;
		virtual void preSolve(float dt) = 0;
		virtual void postSolve(float dt) = 0;

		virtual void setPos(vec3 pos) = 0;
		virtual void setOrient(mat3x3 oriend) = 0;

		virtual bool isSleep() = 0;
		virtual void setSleep(bool flag) = 0;

		virtual void applyImpulse(vec3 point, vec3 impulse) = 0;
		virtual void applyImpulse(phCollisionPoint* collisionPoint) {}
		virtual void addForce(vec3 force) = 0;
		virtual void addTorque(vec3 torque) = 0;

		virtual phCollision* checkCollision(phObject* object, phCollision* collision) = 0;
		virtual phCollisionGeometry* getCollisionGeometry() = 0;
	};
*/
	enum phSleepStage { sleep_stage_sleep = 0, sleep_stage_pre_sleep = 1, sleep_stage_no_sleep = 2 };

	mat3x3 mOrient;
	quat mOrientQuat;
	vec3 mPosition;
	vec3 mVelocity;
	vec3 mAngularVelocity;
	float mMass;
	float mInvMass;
	mat3x3 mInertia;
	mat3x3 mInvInertia;
	mat3x3 mWorldInertia;
	mat3x3 mInvWorldInertia;
	vec3 mForce;
	vec3 mTorque;
	vec3 mBiasVelocity;
	vec3 mBiasAngularVelocity;
	phSleepStage mSleeping;

	phCollisionGeometry* mCollisionGeometry;

	phRigidObject();
	phRigidObject(phScene* scene);
	phRigidObject(vec3 position, mat3x3 orientation, float mass, mat3x3 inertia, phCollisionGeometry* collisionGeometry);
	~phRigidObject();

	void update(float dt);
	void preSolve(float dt);
	void postSolve(float dt);

	void setPos(vec3 pos);
	void setOrient(mat3x3 orient);
	vec3 getPos() { return mPosition; }
	mat3x3 getOrient() { return mOrient; }

	bool isSleep();
	void setSleep(bool flag);

	void applyImpulse(vec3 point, vec3 impulse);
	void applyImpulse(phCollisionPoint* collisionPoint);
	void addForce(vec3 force);
	void addTorque(vec3 torque);

	phCollision* checkCollision(phObject* object, phCollision* collision);
	phCollisionGeometry* getCollisionGeometry();
};

#endif //RIGID_OBJECT_H