#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "../util/math/mmath.h"
#include "../util/other/type_intexation.h"

struct grRenderObjectsManager;

struct grRenderObject
{
	DEFINE_TYPE(grRenderObject)

	grRenderObjectsManager* mRenderObjectsManager;
	UniqueType mType;

	grRenderObject():mRenderObjectsManager(NULL) {}
	grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectsManager) {}

	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual void update(float dt) {  }
	virtual void render() {  }
};


#endif //RENDER_OBJECT_H