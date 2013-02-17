#ifndef PHYSICS_SCENE_H
#define PHYSICS_SCENE_H

#include <vector>

#include "util/math/mmath.h"

struct phEngine;
struct phObject;
struct phCollisionManager;
struct phConstraintsSolverInterface;

struct phScene
{
//types
	typedef std::vector<phObject*> ObjectsList;

//parametres
	ObjectsList                   mObjects;

	phEngine*                     mEngine;
	phCollisionManager*           mCollisionManager;
	phConstraintsSolverInterface* mSolver;

	vec3                          mGravity;

	float                         mPerformance;

//functions
	phScene(phEngine* engine = NULL);
	~phScene();

	phObject* addObject(phObject* object);
	bool removeObject(phObject* object);
	void removeAllObjects();

	void setupSolver(phConstraintsSolverInterface* solver);

	void update(float dt);
};


#endif //PHYSICS_SCENE_H