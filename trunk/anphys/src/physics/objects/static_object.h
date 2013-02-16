#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H


#include "physics_object.h"


struct phStaticObject:public phObject
{
	/* from  phObject
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

		virtual bool isSleep() { return false; }
		virtual void setSleep(bool flag) {}

		virtual void applyImpulse(vec3 point, vec3 impulse) {}
		virtual void applyImpulse(phCollisionPoint* collisionPoint) {}
		virtual void addForce(vec3 force) {}
		virtual void addTorque(vec3 torque) {}

		virtual phCollision* checkCollision(phObject* object, phCollision* collision) { return NULL; }
		virtual phCollisionGeometry* getCollisionGeometry() { return NULL; }
	};
*/
	mat3x3 mOrient;
	vec3 mPosition;
	phCollisionGeometry* mCollisionGeometry;

	phStaticObject();
	phStaticObject(phScene* scene);
	phStaticObject(vec3 position, mat3x3 orientation, phCollisionGeometry* collisionGeometry);
	~phStaticObject();

	void update(float dt);

	void preSolve(float dt);
	void postSolve(float dt);

	void setPos(vec3 pos);
	void setOrient(mat3x3 orient);
	vec3 getPos() { return mPosition; }
	mat3x3 getOrient() { return mOrient; }

	bool isSleep() { return true; }

	phCollision* checkCollision(phObject* object, phCollision* collision);
	phCollisionGeometry* getCollisionGeometry();
};

#endif //STATIC_OBJECT_H