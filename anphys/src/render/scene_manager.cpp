#include "scene_manager.h"

#include "render.h"
#include "render_base_interface.h"
#include "render_scene_base_interface.h"

#include <algorithm>


grSceneManager::grSceneManager(grRenderBaseInterface* render)
{
	mRender = static_cast<grRender*>(render);
}

grRenderSceneBaseInterface* grSceneManager::addScene(grRenderSceneBaseInterface* newScene)
{
	mScenes.push_back(newScene);
	newScene->setSceneManager(this);
	return newScene;
}

bool grSceneManager::removeScene(grRenderSceneBaseInterface* scene)
{
	ScenesList::iterator it = std::find(mScenes.begin(), mScenes.end(), scene);

	if (it == mScenes.end()) return false;

	safe_release(scene);
	mScenes.erase(it);
	
	return true;
}

void grSceneManager::removeAllScenes()
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		safe_release(*it);

	mScenes.clear();
}

void grSceneManager::update(float dt)
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		(*it)->update(dt);
}

void grSceneManager::render()
{
	for (ScenesList::iterator it = mScenes.begin(); it != mScenes.end(); it++)
		(*it)->render();
}

