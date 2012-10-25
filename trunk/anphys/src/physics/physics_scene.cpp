#include "physics_scene.h"

#include <algorithm>

#include "../util/utils.h"

#include "collision_manager.h"
#include "physics_object.h"

phScene::phScene( phEngine* engine /*= NULL*/ ):mEngine(engine), mPerformance(1.0f)
{
	mCollisionManager = new phCollisionManager(this);
	mSolver = NULL; //create solver
}

phScene::~phScene()
{
	safe_release(mCollisionManager);
	safe_release(mSolver);
	removeAllObjects();
}

phObject* phScene::addObject( phObject* object )
{
	mObjects.push_back(object);
	object->mScene = this;
	return object;
}

bool phScene::removeObject( phObject* object )
{
	ObjectsList::iterator fnd = std::find(mObjects.begin(), mObjects.end(), object);
	if (fnd == mObjects.end()) return false;

	safe_release(object);
	mObjects.erase(fnd);

	return true;
}

void phScene::removeAllObjects()
{
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		safe_release(*it);

	mObjects.clear();
}

void phScene::update( float dt )
{
	mCollisionManager->checkCollisions();
	
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		(*it)->preSolve(dt);

	//mSolver->solveConstraints(mPerformance);

	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		(*it)->postSolve(dt);
}

void phScene::setupSolver( phSolver* solver )
{
	safe_release(mSolver);
	mSolver = solver;
}
