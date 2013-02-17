#ifndef RENDER_3D_OBJECT_BASE_INTERFACE_H
#define RENDER_3D_OBJECT_BASE_INTERFACE_H

#include "render/render_objects/render_object_base_interface.h"
#include "render/render_objects/object_manager.h"

struct grRender3DObjectBaseInterface:public grRenderObjectBaseInterface
{
	/* from grRenderObjectBaseInterface
	        {
				grRenderObject
				{
					enum object_types {render_3d_object = 0, render_3d_object_mesh = 1 };
					grRenderObjectsManager* mRenderObjectsManager;
					object_types mType;

					grRenderObject():mRenderObjectsManager(NULL) {}
					grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectManager) {}

					object_types getType() { return mType; }
				}

				grRenderObjectBaseInterface():grRenderObject() {}
				grRenderObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}

				virtual void update(float dt) {  }
				virtual void render() {  }

				virtual std::string getStructRealization() { return "base render object"; }
			}		 
	*/

	vec3 mPosition;
	mat3x3 mOrient;

	mat4x4 mTransform;

	grRender3DObjectBaseInterface():grRenderObjectBaseInterface() {}
	grRender3DObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObjectBaseInterface(objectsManager) {}
	
	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual void render() { }
	virtual void update(float dt) { mTransform = m3to4(mOrient, mPosition); }

	virtual std::string getStructRealization() { return "grRender3DObjectBaseInterface"; }
};

#endif //RENDER_3D_OBJECT_BASE_INTERFACE_H