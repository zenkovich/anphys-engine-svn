#ifndef RENDER_3D_OBJECT_MESH_D3D8_H
#define RENDER_3D_OBJECT_MESH_D3D8_H

#include "render/render_objects/3d/mesh/render_3d_object_mesh_base_interface.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grMeshDataManager;
struct grMeshDataPart;
struct grSurfaceMaterial;

struct grRender3DObjectMeshBase:public grRender3DObjectMeshBaseInterface
{
	grMeshDataManager *mMeshData;

	int mNeedCreateVertCount;
	int mNeedCreatePolyCount;

//functions
	grRender3DObjectMeshBase();
	grRender3DObjectMeshBase(grRenderObjectsManager* objectsManager, int maxVerticies, int maxPolygons);
	~grRender3DObjectMeshBase();

	void setObjectManager(grRenderObjectsManager* objectsManager);

	grMeshPartBaseInterface* addPart(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
									 grSurfaceMaterial* surfaceMaterial);
	bool removePart(grMeshPartBaseInterface* part);

	void update(float dt);
	void render();
};

#endif //RENDER_3D_OBJECT_MESH_D3D8_H