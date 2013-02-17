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
	/* from grRender3DObjectMeshBaseInterface
			{
				grRender3DObjectBaseInterface
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

							object_types getType() { return mType; }
						}

						grRenderObjectBaseInterface():grRenderObject() {}
						grRenderObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}

						virtual void update(float dt) {  }
						virtual void render() {  }

						virtual std::string getStructRealization() { return "base render object"; }
					}	

					vec3 mPosition;
					mat3x3 mOrient;

					mat4x4 mTransform;

					grRender3DObjectBaseInterface():grRenderObjectBaseInterface() {}
					grRender3DObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObjectBaseInterface(objectsManager) {}

					virtual void render() { }
					virtual void update(float dt) { mTransform = m3to4(mOrient, mPosition); }

					virtual std::string getStructRealization() { return "grRender3DObjectBaseInterface"; }
				}

				typedef std::vector<grMeshPartBaseInterface*> PartsList;

				PartsList mParts;

				grRender3DObjectMeshBaseInterface():grRender3DObjectBaseInterface() {}
				grRender3DObjectMeshBaseInterface(grRenderObjectsManager* objectsManager, int maxVerticies, int maxPolygons):grRender3DObjectBaseInterface(objectsManager) {}

				grMeshPartBaseInterface* addPart(grMeshPartBaseInterface* newPart);
				bool removePart(grMeshPartBaseInterface* part);

				void update(float dt);
				void render();

				std::string getStructRealization() { return "grRender3DObjectMeshBaseInterface"; }
			} */
	
	grMeshDataManager *mMeshData;

	int mNeedCreateVertCount;
	int mNeedCreatePolyCount;

	grRender3DObjectMeshBase();
	grRender3DObjectMeshBase(grRenderObjectsManager* objectsManager, int maxVerticies, int maxPolygons);
	~grRender3DObjectMeshBase();

	void setObjectManager(grRenderObjectsManager* objectsManager);

	grMeshPartBaseInterface* addPart(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
									 grSurfaceMaterial* surfaceMaterial);
	bool removePart(grMeshPartBaseInterface* part);

	void update(float dt);
	void render();

	std::string getStructRealization() { return "grRender3DObjectMeshBaseInterface"; }
};

#endif //RENDER_3D_OBJECT_MESH_D3D8_H