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

	virtual void applyImpulse(vec3 point, vec3 impulse) {}
	virtual void applyImpulse(phCollisionPoint* collisionPoint) {}
	virtual void addForce(vec3 force) {}
	virtual void addTorque(vec3 torque) {}

	virtual phCollision* checkCollision(phObject* object, phCollision* collision) { return NULL; }
	virtual phCollisionGeometry* getCollisionGeometry() { return NULL; }
};


#endif //PHYSICS_OBJECT_H