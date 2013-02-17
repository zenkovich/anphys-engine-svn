#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "mesh_data_manager_d3d8.h"
#include "render/render.h"

#include <algorithm>

grMeshDataManager::grMeshDataManager(unsigned long int aviableVertexCount, 
									  unsigned long int aviablePolygonsCount,
									  LPDIRECT3DDEVICE8 devicePtr,
									  unsigned long int maxVertexCount, 
									  unsigned long int maxPolygonsCount,
									  bool resizeIfNeed)
{
	mResizeIfNeed = resizeIfNeed;

	assert(aviableVertexCount <= maxVertexCount, "");
	assert(aviablePolygonsCount <= maxPolygonsCount, "");

	mMaxVertexCount = maxVertexCount;
	mMaxPolygonsCount = maxPolygonsCount;

	mAviableVertexCount = aviableVertexCount;
	mAviablePolygonsCount = aviablePolygonsCount;

	mVertexCount = 0;
	mPolygonsCount = 0;

	mLocked = false;

	mDevicePtr = devicePtr;

	mDevicePtr->CreateVertexBuffer(mAviableVertexCount*sizeof(vertexTexNorm), 0, 
		D3DFVF_VERTEX_TEXTURE_LIGHT, D3DPOOL_DEFAULT, &mVertexBuffer); 
	mDevicePtr->CreateIndexBuffer(maxPolygonsCount*3*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer);
}

grMeshDataManager::~grMeshDataManager()
{
	if (mLocked) unlockVertexData();

	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		safe_release(*it);

	mVertexBuffer->Release();
	mIndexBuffer->Release();
}

bool grMeshDataManager::addVerticies(vertexTexNorm* verticies, int vertCount)
{
	if (vertCount + mVertexCount >= mAviableVertexCount) 
		if (mResizeIfNeed) resizeVertexBuffer(vertCount + mVertexCount + 1);
		else return false;

	vertexTexNorm* vertexBuffer = lockVertexData(mVertexCount, mVertexCount + vertCount);

	if (!vertexBuffer) return false;


	copyCycle(vertexBuffer, verticies, vertCount);

	assert(unlockVertexData(), "");

	mVertexCount += vertCount;

	return true;
}

grMeshDataPart* grMeshDataManager::addPart(int* indexes, int polyCount)
{
	if (polyCount + mPolygonsCount >= mAviablePolygonsCount) 
		if (mResizeIfNeed) resizeIndexBuffer(polyCount + mPolygonsCount);
		else return NULL;

	grMeshDataPart* newPart = new grMeshDataPart(mPolygonsCount, mPolygonsCount + polyCount);

	WORD* indexBuffer = lockIndexData(mPolygonsCount*3, (mPolygonsCount + polyCount)*3);
	if (!indexBuffer) return false;

	copyCycle(indexBuffer, indexes, polyCount*3);

	assert(unlockIndexData(), "");

	mParts.push_back(newPart);

	mPolygonsCount += polyCount;

	return newPart;
}

grMeshDataPart* grMeshDataManager::addPart(poly3* polygons, int polyCount)
{
	if (polyCount + mPolygonsCount >= mAviablePolygonsCount) 
		if (mResizeIfNeed) resizeIndexBuffer(polyCount + mPolygonsCount);
		else return NULL;

	grMeshDataPart* newPart = new grMeshDataPart(mPolygonsCount, mPolygonsCount + polyCount);

	WORD* indexBuffer = lockIndexData(mPolygonsCount*3, (mPolygonsCount + polyCount)*3);
	if (!indexBuffer) return false;

	for (int i = 0; i < polyCount; i++)
	{
		indexBuffer[i*3] = polygons[i].a;
		indexBuffer[i*3 + 1] = polygons[i].b;
		indexBuffer[i*3 + 2] = polygons[i].c;
	}

	assert(unlockIndexData(), "");

	mParts.push_back(newPart);

	return newPart;
}

grMeshDataPart* grMeshDataManager::addPart(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount)
{
	for (int i = 0; i < polyCount*3; i++)
		indexes[i] += mVertexCount;

	addVerticies(verticies, vertCount);

	return addPart(indexes, polyCount);
}

bool grMeshDataManager::removePart(grMeshDataPart* part)
{	
	PartsList::iterator it = std::find(mParts.begin(), mParts.end(), part);
	assert(it == mParts.end(), "");

	mParts.erase(it);

	return true;
}

bool grMeshDataManager::removePart(int idx)
{
	if (idx >= (int)mParts.size()) return false;

	mParts.erase(mParts.begin() + idx);

	return true;
}

bool grMeshDataManager::removeAllParts()
{
	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		safe_release(*it);

	mParts.clear();

	return true;
}

bool grMeshDataManager::clearData()
{
	removeAllParts();

	mIndexBuffer->Release();  mIndexBuffer = NULL;
	mVertexBuffer->Release(); mVertexBuffer = NULL;

	return true;
}

vertexTexNorm* grMeshDataManager::lockVertexData(unsigned long start, unsigned long end)
{
	if (end <= start) end = mAviableVertexCount;

	assert(end <= mAviableVertexCount, "");

	vertexTexNorm *pVB;
	assert(!FAILED(mVertexBuffer->Lock(start, end - start, (BYTE**)&pVB, 0)), "");

	mLocked = true;

	return pVB;
}

bool grMeshDataManager::unlockVertexData()
{
	if (!mLocked) return false;

	assert(!FAILED(mVertexBuffer->Unlock()), "");

	return true;
}
	
WORD* grMeshDataManager::lockIndexData(unsigned long start, unsigned long end)
{
	if (end <= start) end = mAviablePolygonsCount;

	assert(end <= mAviablePolygonsCount*3, "");

	WORD *pIB;
	assert(!FAILED(mIndexBuffer->Lock(start*3, (end - start)*3, (BYTE**)&pIB, 0)), "");

	mLocked = true;

	return pIB;
}
	
bool grMeshDataManager::unlockIndexData()
{
	if (!mLocked) return false;

	assert(!FAILED(mIndexBuffer->Unlock()), "");

	return true;
}
	
bool grMeshDataManager::drawPart(grMeshDataPart* part, LPDIRECT3DDEVICE8 device, D3DPRIMITIVETYPE primtiveType)
{
	device->DrawIndexedPrimitive(primtiveType, part->mStart*3, mVertexCount, part->mStart*3, part->mEnd - part->mStart);
	return true;
}
	
bool grMeshDataManager::activeIndexes(LPDIRECT3DDEVICE8 device)
{	
	device->SetVertexShader(D3DFVF_VERTEX_TEXTURE_LIGHT);
	device->SetStreamSource(0, mVertexBuffer, sizeof(vertexTexNorm));
	device->SetIndices(mIndexBuffer, 0);
	return true;
}
	 
bool grMeshDataManager::resizeVertexBuffer(unsigned long int newSize)
{
	//@@@@@@@
	return false;
}

bool grMeshDataManager::resizeIndexBuffer(unsigned long int newSize)
{
	//@@@@
	return false;
}


#endif //RENDER_D3D8
