#ifndef PHYSICS_SCENE_H
#define PHYSICS_SCENE_H

#include <vector>

#include "../util/math/mmath.h"

struct phEngine;
struct phObject;
struct phCollisionManager;
struct phSolver;

struct phScene
{
	typedef std::vector<phObject*> ObjectsList;

	ObjectsList         mObjects;

	phEngine*           mEngine;
	phCollisionManager* mCollisionManager;
	phSolver*           mSolver;

	vec3                mGravity;

	float mPerformance;


	phScene(phEngine* engine = NULL);
	~phScene();

	phObject* addObject(phObject* object);
	bool removeObject(phObject* object);
	void removeAllObjects();

	void setupSolver(phSolver* solver);

	void update(float dt);
};


#endif //PHYSICS_SCENE_H