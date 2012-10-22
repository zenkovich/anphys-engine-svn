#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "../util/math/mmath.h"

struct grRenderObjectsManager;

struct grRenderObject
{
	enum object_types {render_3d_object = 0, render_3d_object_mesh = 1 };
	grRenderObjectsManager* mRenderObjectsManager;
	object_types mType;

	grRenderObject():mRenderObjectsManager(NULL) {}
	grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectsManager) {}

	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual void update(float dt) {  }
	virtual void render() {  }

	object_types getType() { return mType; }
};

#endif //RENDER_OBJECT_H