#include "physics_engine.h"

#include <algorithm>

#include "../util/utils.h"

#include "physics_scene.h"

phEngine::phEngine()
{
}

phEngine::~phEngine()
{
	removeAllScenes();
}

phScene* phEngine::addScene( phScene* scene )
{
	mScenes.push_back(scene);
	scene->mEngine = this;
	return scene;
}

bool phEngine::removeScene( phScene* scene )
{
	ScenesList::iterator fnd = std::find(mScenes.begin(), mScenes.end(), scene);
	if (fnd == mScenes.end()) return false;

	safe_release(scene);
	mScenes.erase(fnd);

	return true;
}

void phEngine::removeAllScenes()
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		safe_release(*it);

	mScenes.clear();
}

void phEngine::update( float dt )
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		(*it)->update(dt);
}
