#ifndef _2D_RENDER_STATE_D3D8_H
#define _2D_RENDER_STATE_D3D8_H

#include "render/render_state/2d_render_state_base_interface.h"
#include "util/math/mmath.h"
#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

struct grTexture;

struct gr2DRenderStateBase:gr2DRenderStateBaseInterface
{
	enum { nVertexBufferSize = 4000, nIndexBufferSize = 5000 };

	LPDIRECT3DVERTEXBUFFER8 mVertexBuffer;
	LPDIRECT3DINDEXBUFFER8  mIndexBuffer;
	
	grTexture*              mLastDrawingTexture;
	unsigned int            mLastDrawingVertex;
	unsigned int            mLastDrawingIndex;
	unsigned int            mTrianglesCount;
	unsigned int            mFrameTrianglesCount;

	vertex2d*               mVertexData;
	WORD*                   mIndexData;

	unsigned int            mFrameIdx;

	bool                    mRendering;

//functions
	gr2DRenderStateBase();
	gr2DRenderStateBase(grRenderBase* render);
	virtual ~gr2DRenderStateBase();

	void initialize();

	void begin();
	void finish();

	void bindCamera(grCamera2D* camera);
	void updateTransformations();

	void drawMesh(grRender2DObjectMeshBase* mesh);

	void flush();

	void drawPrimitives();

	void lockBuffers();
	void unlockBuffers();

	void setTexture(grTexture* texture);

	void incFrameIdx() { mFrameIdx += 1; if (mFrameIdx > 999999) mFrameIdx = 0; }
	void resetFrameIdx() { mFrameIdx = 0; }

	void renderLinesData();
};

#endif //2D_RENDER_STATE_D3D8_H