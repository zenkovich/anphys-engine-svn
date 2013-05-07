#ifndef _2D_RENDER_STATE_BASE_INTERFACE_H
#define _2D_RENDER_STATE_BASE_INTERFACE_H

#include "render/render_state/render_state_interface.h"
#include "util/math/mmath.h"

struct grRender2DObjectMeshBase;
struct grCamera2D;

struct gr2DRenderStateBaseInterface:public grRenderState
{
	grCamera2D*     mCamera;
	PolygonCullMode mPolygonCullMode;

	enum { nDebugVertexBufferSize = 4096 };
	vertex2d        mDebugVertexBuffer[nDebugVertexBufferSize];
	unsigned int    mDebugLinesCount;

//functions
	gr2DRenderStateBaseInterface();
	gr2DRenderStateBaseInterface(grRenderBase* render);
	virtual ~gr2DRenderStateBaseInterface();

	virtual void begin() {}
	virtual void finish() {}

	virtual void drawMesh(grRender2DObjectMeshBase* mesh) {}

	virtual void bindCamera(grCamera2D* camera) { mCamera = camera; }

	virtual void flush() {}

	virtual void pushLine(const vec2& start, const vec2& end, const color4& color = color4(0, 255, 0, 255));
	virtual void pushRect(const vec2& leftTop, const vec2& rightDown, const color4& color = color4(0, 255, 0, 255));
};

#endif //2D_RENDER_STATE_BASE_INTERFACE_H