#ifndef MESH_PART_D3D8_H
#define MESH_PART_D3D8_H


#include "render/render_objects/3d/mesh/mesh_part_base_interface.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grMeshDataPart;

struct grMeshPartBase:public grMeshPartBaseInterface
{
	/* from grMeshPartBaseInterface
			{
				grSurfaceMaterial* mSurfaceMaterial;
				grRender3DObjectMeshBase* mMesh;

				vec3 mPosition;
				mat3x3 mOrient;
				mat4x4 mTransform;

				grMeshPartBaseInterface(grRender3DObjectMeshBaseInterface* mesh):mMesh(mesh) {}

				virtual bool create(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
									grSurfaceMaterial* surfaceMaterial) { return false; }

				virtual void update(float dt) {}
				virtual void render() {} 
			}*/
	
	grMeshDataPart* mMeshDataPart;
	bool mReady;

	grMeshPartBase(grRender3DObjectMeshBase* mesh):grMeshPartBaseInterface(mesh), mReady(false), mMeshDataPart(NULL) {}
	~grMeshPartBase();

	virtual bool create(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
		                grSurfaceMaterial* surfaceMaterial);

	virtual void update(float dt);
	virtual void render();
};



#endif //MESH_PART_D3D8_H