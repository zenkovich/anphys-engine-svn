#ifndef SIMPLE_3D_RENDER_STATE_BASE_INTERFACE_H
#define SIMPLE_3D_RENDER_STATE_BASE_INTERFACE_H

#include "render_state_interface.h"
#include "util/math/color.h"

struct grCamera3D;

struct grSimple3DRenderStateBaseInterface:public grRenderState
{
	color4          mAmbientColor;
	PolygonCullMode mPolygonCullMode;

	grCamera3D*     mCamera;

//functions
	grSimple3DRenderStateBaseInterface();
	grSimple3DRenderStateBaseInterface(grRenderBase* render);
	~grSimple3DRenderStateBaseInterface();

	virtual void begin() {}
	virtual void finish() {}

	virtual void bindCamera(grCamera3D* camera) { mCamera = camera; }
};

#endif //SIMPLE_3D_RENDER_STATE_BASE_INTERFACE_H