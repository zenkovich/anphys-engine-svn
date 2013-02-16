#include "../../engine/engine_options.h"

#ifdef RENDER_D3D8
#include "render_3d_object_mesh_d3d8.h"
#include "mesh_data_manager_d3d8.h"
#include "../mesh_part.h"
#include "../render.h"
#include "../object_manager.h"
#include <algorithm>

grRender3DObjectMeshBase::grRender3DObjectMeshBase():grRender3DObjectMeshBaseInterface() 
{
	mMeshData = NULL;
	mNeedCreateVertCount = mNeedCreatePolyCount = 0;
}

grRender3DObjectMeshBase::grRender3DObjectMeshBase(grRenderObjectsManager* objectsManager, 
												   int maxVerticies, int maxPolygons):
	grRender3DObjectMeshBaseInterface(objectsManager)
{
	if (!objectsManager) 
	{
		mNeedCreateVertCount = maxVerticies;
		mNeedCreatePolyCount = maxPolygons;
		mMeshData = NULL;
		return;
	}
	mMeshData = new grMeshDataManager(maxVerticies, maxPolygons, objectsManager->mRender->m_pDirect3DDevice, maxVerticies, maxPolygons, true);

	mNeedCreateVertCount = mNeedCreatePolyCount = 0;
}
	
void grRender3DObjectMeshBase::setObjectManager(grRenderObjectsManager* objectsManager)
{
	mRenderObjectsManager = objectsManager;

	if (mMeshData) safe_release(mMeshData);
	
	mMeshData = new grMeshDataManager(mNeedCreateVertCount, mNeedCreatePolyCount, 
									  objectsManager->mRender->m_pDirect3DDevice, mNeedCreateVertCount, 
		                              mNeedCreatePolyCount, true);
}

grRender3DObjectMeshBase::~grRender3DObjectMeshBase()
{	
	for (grRender3DObjectMeshBaseInterface::PartsList::iterator it = mParts.begin();
		 it != mParts.end(); it++)
	{
		safe_release(*it);
	}
	mParts.clear();
	safe_release(mMeshData);
}

grMeshPartBaseInterface* grRender3DObjectMeshBase::addPart(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
														   grSurfaceMaterial* surfaceMaterial)
{
	grMeshPart* newPart = new grMeshPart(this);
	newPart->create(verticies, vertCount, indexes, polyCount, surfaceMaterial);
	mParts.push_back(newPart);
	return newPart;
}

bool grRender3DObjectMeshBase::removePart(grMeshPartBaseInterface* part)
{
	grRender3DObjectMeshBaseInterface::PartsList::iterator it = std::find(mParts.begin(), mParts.end(), part);

	if (it == mParts.end()) return false;

	safe_release(*it);
	mParts.erase(it);

	return true;
}

void grRender3DObjectMeshBase::update(float dt)
{
	for (grRender3DObjectMeshBaseInterface::PartsList::iterator it = mParts.begin();
		 it != mParts.end(); it++)
	{
		(*it)->update(dt);
	}
}

void grRender3DObjectMeshBase::render()
{
	mMeshData->activeIndexes(mRenderObjectsManager->mRender->m_pDirect3DDevice);

	for (grRender3DObjectMeshBaseInterface::PartsList::iterator it = mParts.begin();
		 it != mParts.end(); it++)
	{
		(*it)->render();
	}
}


#endif //RENDER_D3D8
