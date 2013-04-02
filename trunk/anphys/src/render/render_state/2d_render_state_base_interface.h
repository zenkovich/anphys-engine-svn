#ifndef _2D_RENDER_STATE_BASE_INTERFACE_H
#define _2D_RENDER_STATE_BASE_INTERFACE_H

#include "render/render_state/render_state_interface.h"

struct grRender2DObjectMeshBase;
struct grCamera2D;

struct gr2DRenderStateBaseInterface:public grRenderState
{
	grCamera2D*     mCamera;
	PolygonCullMode mPolygonCullMode;

//functions
	gr2DRenderStateBaseInterface();
	gr2DRenderStateBaseInterface(grRenderBase* render);
	virtual ~gr2DRenderStateBaseInterface();

	virtual void begin() {}
	virtual void finish() {}

	virtual void drawMesh(grRender2DObjectMeshBase* mesh) {}
};

#endif //2D_RENDER_STATE_BASE_INTERFACE_H