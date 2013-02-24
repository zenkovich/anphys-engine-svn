#ifndef RENDER_2D_D3D8_H
#define RENDER_2D_D3D8_H

#include "util/math/mmath.h"

#include "render/2d_render/render_2d_base_interface.h"

#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grTexture;

struct grRender2DBase:public grRender2DBaseInterface
{
	enum { nVertexBufferSize = 16384, nIndexBufferSize = 49152 };

	LPDIRECT3DVERTEXBUFFER8 mVertexBuffer;
	LPDIRECT3DINDEXBUFFER8  mIndexBuffer;
	D3DXMATRIX              mProjMatrix;
	D3DXMATRIX              mViewMatrix;
	D3DXMATRIX              mLastProjMatrix;
	D3DXMATRIX              mLastViewMatrix;

	grTexture*              mLastDrawingTexture;
	unsigned int            mLastDrawingVertex;
	unsigned int            mLastDrawingIndex;
	unsigned int            mTrianglesCount;
	unsigned int            mFrameTrianglesCount;

	vertex2d*               mVertexData;
	WORD*                   mIndexData;

	unsigned int            mFrameIdx;

//functions

	grRender2DBase(grRenderBaseInterface* render);
	~grRender2DBase();

	void initialize();

	void beginRender();
	void endRender();

	void drawMesh(grRender2DObjectMeshBase* renderMesh);
	void drawPrimitives();

	void lockBuffers();
	void unlockBuffers();

	void updateMatrix();
	void setupMatrix();
	void revertMatrix();

	void setTexture(grTexture* texture);

	void incFrameIdx() { mFrameIdx += 1; if (mFrameIdx > 999999) mFrameIdx = 0; }
	void resetFrameIdx() { mFrameIdx = 0; }
};

#endif //RENDER_2D_D3D8_H