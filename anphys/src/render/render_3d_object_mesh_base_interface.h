#ifndef RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H
#define RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H

#include "render_3d_object_base_interface.h"
#include <vector>

struct grMeshPartBaseInterface;

struct grRender3DObjectMeshBaseInterface:public grRender3DObjectBaseInterface
{
	/* from grRender3DObjectBaseInterface
			{
				grRenderObjectBaseInterface
				{
					grRenderObject
					{
						enum object_types {render_3d_object = 0, render_3d_object_mesh = 1 };
						grRenderObjectsManager* mRenderObjectsManager;
						object_types mType;

						grRenderObject():mRenderObjectsManager(NULL) {}
						grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectManager) {}

						
						virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

						object_types getType() { return mType; }
					}

					grRenderObjectBaseInterface():grRenderObject() {}
					grRenderObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}
					
					virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

					virtual void update(float dt) {  }
					virtual void render() {  }

					virtual std::string getStructRealization() { return "base render object"; }
				}	

				vec3 mPosition;
				mat3x3 mOrient;

				mat4x4 mTransform;

				grRender3DObjectBaseInterface():grRenderObjectBaseInterface() {}
				grRender3DObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObjectBaseInterface(objectsManager) {}

				virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

				virtual void render() { }
				virtual void update(float dt) { mTransform = m3to4(mOrient, mPosition); }

				virtual std::string getStructRealization() { return "grRender3DObjectBaseInterface"; }
			}*/

	typedef std::vector<grMeshPartBaseInterface*> PartsList;

	PartsList mParts;

	grRender3DObjectMeshBaseInterface():grRender3DObjectBaseInterface() {}
	grRender3DObjectMeshBaseInterface(grRenderObjectsManager* objectsManager, int maxVerticies = 0, int maxPolygons = 0):grRender3DObjectBaseInterface(objectsManager) {}

	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual grMeshPartBaseInterface* addPart(grMeshPartBaseInterface* newPart) { return NULL; }
	virtual bool removePart(grMeshPartBaseInterface* part) { return false; }

	virtual void update(float dt) {}
	virtual void render() {}

	virtual std::string getStructRealization() { return "grRender3DObjectMeshBaseInterface"; }
};

#endif //RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H