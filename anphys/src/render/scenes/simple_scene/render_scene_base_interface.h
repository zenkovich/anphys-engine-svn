#ifndef RENDER_SCENE_BASE_INTERFACE_H
#define RENDER_SCENE_BASE_INTERFACE_H

#include <vector>

#include "util/utils.h"

struct grRenderObject;
struct grSceneManager;
struct grRenderObjectsManager;

struct grRenderSceneBaseInterface
{
	grSceneManager*         mSceneManager;
	grRenderObjectsManager* mObjects;

//functions
	grRenderSceneBaseInterface(grSceneManager* sceneManager = NULL);
	virtual ~grRenderSceneBaseInterface();

	void setSceneManager(grSceneManager* sceneManager);

	void update(float dt);
	void render();
};


#endif //RENDER_SCENE_BASE_INTERFACE_H