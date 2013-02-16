#ifndef MESH_PART_BASE_INTERFACE_H
#define MESH_PART_BASE_INTERFACE_H

#include "../util/utils.h"

struct grSurfaceMaterial;
struct grRender3DObjectMeshBase;

struct grMeshPartBaseInterface
{
	grSurfaceMaterial* mSurfaceMaterial;
	grRender3DObjectMeshBase* mMesh;

	vec3 mPosition;
	mat3x3 mOrient;
	mat4x4 mTransform;

	grMeshPartBaseInterface(grRender3DObjectMeshBase* mesh):mMesh(mesh) {}

	virtual bool create(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
		                grSurfaceMaterial* surfaceMaterial) { return false; }

	virtual void update(float dt) {}
	virtual void render() {}
};

#endif //MESH_PART_BASE_INTERFACE_H