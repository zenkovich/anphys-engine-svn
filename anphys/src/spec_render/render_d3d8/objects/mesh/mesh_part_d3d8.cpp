#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "mesh_part_d3d8.h"
#include "render_3d_object_mesh_d3d8.h"
#include "mesh_data_manager_d3d8.h"
#include "render/render.h"
#include "render/surface/surface_material.h"

grMeshPartBase::~grMeshPartBase()
{
	mMesh->mMeshData->removePart(mMeshDataPart);
}

bool grMeshPartBase::create(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount, 
		                grSurfaceMaterial* surfaceMaterial)
{
	assert(mMesh, "mMesh in meshPart is NULL");

	safe_release(mMeshDataPart);

	mMeshDataPart = mMesh->mMeshData->addPart(verticies, vertCount, indexes, polyCount);

	if (!mMeshDataPart) return NULL;

	mSurfaceMaterial = surfaceMaterial;

	mReady = true;

	return true;
}

void grMeshPartBase::update(float dt)
{
	mTransform = m3to4(mMesh->mOrient*mOrient, mMesh->mPosition + mPosition*mMesh->mOrient);
}

void grMeshPartBase::render()
{
	if (!mReady) return;

	D3DXMATRIX transf;
	transf._11=mTransform[0][0]; transf._12=mTransform[0][1]; transf._13=mTransform[0][2]; transf._14=mTransform[0][3];
	transf._21=mTransform[1][0]; transf._22=mTransform[1][1]; transf._23=mTransform[1][2]; transf._24=mTransform[1][3];
	transf._31=mTransform[2][0]; transf._32=mTransform[2][1]; transf._33=mTransform[2][2]; transf._34=mTransform[2][3];
	transf._41=mTransform[3][0]; transf._42=mTransform[3][1]; transf._43=mTransform[3][2]; transf._44=mTransform[3][3];
	mMesh->mRenderObjectsManager->mRender->m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &transf);

	if (mSurfaceMaterial) mSurfaceMaterial->setup();

	mMesh->mMeshData->drawPart(mMeshDataPart, mMesh->mRenderObjectsManager->mRender->m_pDirect3DDevice);
}



#endif //RENDER_D3D8