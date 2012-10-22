#ifndef MESH_DATA_MANAGER_D3D8_H
#define MESH_DATA_MANAGER_D3D8_H

#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

#include "../../util/utils.h"

#include <vector>

#define MAX_VERTEX_COUNT 36000
#define MAX_POLYGON_COUNT 36000

struct grMeshDataPart
{
	unsigned long int mStart;
	unsigned long int mEnd;

	grMeshDataPart():mStart(0), mEnd(0) {}
	grMeshDataPart(unsigned long int start, unsigned long int end):mStart(start), mEnd(end) {}
};

struct grMeshDataManager
{
	LPDIRECT3DDEVICE8 mDevicePtr;
	LPDIRECT3DVERTEXBUFFER8 mVertexBuffer;
	LPDIRECT3DINDEXBUFFER8  mIndexBuffer;

	unsigned long int mVertexCount;
	unsigned long int mPolygonsCount;
	unsigned long int mMaxVertexCount;
	unsigned long int mMaxPolygonsCount;
	unsigned long int mAviableVertexCount;
	unsigned long int mAviablePolygonsCount;

	typedef std::vector<grMeshDataPart*> PartsList;
	PartsList mParts;

	bool mLocked;
	bool mResizeIfNeed;

	grMeshDataManager(LPDIRECT3DDEVICE8 devicePtr,
					  unsigned long int maxVertexCount = MAX_VERTEX_COUNT, 
		              unsigned long int maxPolygonsCount = MAX_POLYGON_COUNT,
					  bool resizeIfNeed = false):
				mMaxVertexCount(maxVertexCount), mMaxPolygonsCount(maxPolygonsCount), mVertexCount(0),
				mPolygonsCount(0), mAviableVertexCount(0), mAviablePolygonsCount(0), mVertexBuffer(NULL),
				mIndexBuffer(NULL), mLocked(false), mDevicePtr(devicePtr), mResizeIfNeed(resizeIfNeed) {};

	grMeshDataManager(unsigned long int aviableVertexCount, 
					  unsigned long int aviablePolygonsCount,
					  LPDIRECT3DDEVICE8 devicePtr,
					  unsigned long int maxVertexCount = MAX_VERTEX_COUNT, 
		              unsigned long int maxPolygonsCount = MAX_POLYGON_COUNT,
					  bool resizeIfNeed = false);

	~grMeshDataManager();

	bool addVerticies(vertexTexNorm* verticies, int vertCount);
	grMeshDataPart* addPart(int* indexes, int polyCount);
	grMeshDataPart* addPart(poly3* polygons, int polyCount);
	grMeshDataPart* addPart(vertexTexNorm* verticies, int vertCount, int* indexes, int polyCount);

	bool removePart(grMeshDataPart* part);
	bool removePart(int idx);
	bool removeAllParts();
	bool clearData();

	vertexTexNorm* lockVertexData(unsigned long start = 0, unsigned long end = 0);
	bool unlockVertexData();

	WORD* lockIndexData(unsigned long start = 0, unsigned long end = 0);
	bool unlockIndexData();

	bool resizeVertexBuffer(unsigned long int newSize);
	bool resizeIndexBuffer(unsigned long int newSize);

	bool drawPart(grMeshDataPart* part, LPDIRECT3DDEVICE8 device, D3DPRIMITIVETYPE primtiveType = D3DPT_TRIANGLELIST);
	bool activeIndexes(LPDIRECT3DDEVICE8 device);
};


#endif //MESH_DATA_MANAGER_D3D8_H