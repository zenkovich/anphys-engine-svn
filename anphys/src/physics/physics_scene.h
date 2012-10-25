#ifndef PHYSICS_SCENE_H
#define PHYSICS_SCENE_H

#include <vector>

struct phEngine;
struct phObject;
struct phCollisionManager;
struct phSolver;

struct phScene
{
	typedef std::vector<phObject*> ObjectsList;

	ObjectsList mObjects;

	phEngine* mEngine;
	phCollisionManager* mCollisionManager;
	phSolver* mSolver;

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