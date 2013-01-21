#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "../util/utils.h"

struct phScene;
struct phCollision;
struct phCollisionPoint;
struct phCollisionGeometry;

struct phObject
{
	enum phObjectType { rigid_physics_object = 0, static_physics_object = 1 };
	phObjectType mType;

	phScene* mScene;

	phObject():mScene(NULL) {}
	phObject(phScene* scene):mScene(scene) {}

	virtual void update(float dt) {}
	virtual void preSolve(float dt) {}
	virtual void postSolve(float dt) {}

	virtual void setPos(vec3 pos) {}
	virtual void setOrient(mat3x3 oriend) {}
	virtual vec3 getPos() { return vec3(0); }
	virtual mat3x3 getOrient() { return nullMatr(); }

	virtual bool isSleep() { return false; }
	virtual void setSleep(bool flag) {}
	
	virtual void applyImpulse(vec3& point, vec3& impulse) {}
	virtual void applyImpulse(vec3& point, vec3& impulse, vec3& biasImpulse) {}
	virtual void applyImpulse(phCollisionPoint* collisionPoint) {}
	virtual void applyBiasImpulse(vec3& point, vec3& impulse) {}
	virtual void addForce(vec3& force) {}
	virtual void addTorque(vec3& torque) {}

	virtual vec3 getVelocity() { return vec3(0); }
	virtual vec3 getAngularVelocity() { return vec3(0); }
	virtual vec3 getVelocity(vec3& point) { return vec3(0); }
	virtual vec3 getBiasVelocity(vec3& point) { return vec3(0); }
	virtual vec3 getBiasAngularVelocity() { return vec3(0); }
	virtual vec3 getBiasVelocity() { return vec3(0); }
	virtual mat3x3 getInvertedInertia() { return mat3x3(0, 0, 0, 0, 0, 0, 0, 0, 0); }
	virtual float getInvertedMass() { return 0; }

	virtual float getMass() { return 0; }

	virtual phCollision* checkCollision(phObject* object, phCollision* collision) { return NULL; }
	virtual phCollisionGeometry* getCollisionGeometry() { return NULL; }
};


#endif //PHYSICS_OBJECT_H