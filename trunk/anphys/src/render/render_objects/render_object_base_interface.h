#ifndef RENDER_OBJECT_BASE_INTERFACE_H
#define RENDER_OBJECT_BASE_INTERFACE_H

#include <string>

#include "util/math/mmath.h"
#include "util/log/log_system.h"
#include "render_object.h"

struct grRenderObjectBaseInterface:public grRenderObject
{
	/* from grRenderObject
			{
				enum object_types {render_3d_object = 0, render_3d_object_mesh = 1 };
				grRenderObjectsManager* mRenderObjectsManager;
				object_types mType;

				virtual void update(float dt) {  }
				virtual void render() {  }

				grRenderObject():mRenderObjectsManager(NULL) {}
				grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectManager) {}

				object_types getType() { return mType; }
			}*/

	grRenderObjectBaseInterface():grRenderObject() {}
	grRenderObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}

	virtual void update(float dt) {  }
	virtual void render() {  }

	virtual std::string getStructRealization() { return "base render object"; }
};

#endif //RENDER_OBJECT_BASE_INTERFACE_H
