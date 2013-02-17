#include "physics_scene.h"

#include <algorithm>

#include "util/utils.h"

#include "../CD/collision_manager.h"
#include "../objects/physics_object.h"
#include "../solver/sequential_impulses_solver.h"

phScene::phScene( phEngine* engine /*= NULL*/ ):mEngine(engine), mPerformance(1.0f)
{
	mCollisionManager = new phCollisionManager(this);
	mSolver = new phSequentialImpulsesSolver(this);
	mGravity = vec3(0, -9.8f, 0);
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
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		(*it)->postSolve(dt);

	mCollisionManager->checkCollisions();

	vec3 gravityVec = mGravity*dt;

	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
	{
		(*it)->addForce(gravityVec);
		(*it)->preSolve(dt);
	}

	if (mSolver) mSolver->solveConstraints(mPerformance, dt);
}

void phScene::setupSolver( phConstraintsSolverInterface* solver )
{
	safe_release(mSolver);
	mSolver = solver;
}
