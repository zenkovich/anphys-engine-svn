#ifndef RENDER_SCENE_BASE_INTERFACE_H
#define RENDER_SCENE_BASE_INTERFACE_H

#include "../util/utils.h"

#include <vector>

struct grRenderObject;
struct grSceneManager;
struct grRenderObjectsManager;

struct grRenderSceneBaseInterface
{
	grSceneManager* mSceneManager;

	grRenderObjectsManager* mObjects;

	//fuck yeah

	grRenderSceneBaseInterface(grSceneManager* sceneManager = NULL);
	~grRenderSceneBaseInterface();

	void setSceneManager(grSceneManager* sceneManager);

	void update(float dt);
	void render();
};


#endif //RENDER_SCENE_BASE_INTERFACE_H