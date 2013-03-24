#ifndef RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H
#define RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H

#include <vector>
#include "render/render_objects/3d/render_3d_object.h"

struct grMeshPartBaseInterface;

struct grRender3DObjectMeshBaseInterface:public grRender3DObject
{
	typedef std::vector<grMeshPartBaseInterface*> PartsList;

	PartsList mParts;

//functions
	grRender3DObjectMeshBaseInterface():
		grRender3DObject() {}

	grRender3DObjectMeshBaseInterface(grRenderObjectsManager* objectsManager, int maxVerticies = 0, int maxPolygons = 0):
		grRender3DObject(objectsManager) {}

	virtual void setObjectManager(grRenderObjectsManager* objectsManager) {}

	virtual grMeshPartBaseInterface* addPart(grMeshPartBaseInterface* newPart) { return NULL; }
	virtual bool removePart(grMeshPartBaseInterface* part) { return false; }

	virtual void update(float dt) {}
	virtual void render() {}
};

#endif //RENDER_3D_OBJECT_MESH_BASE_INTERFACE_H