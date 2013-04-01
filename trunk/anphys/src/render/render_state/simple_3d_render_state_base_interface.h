#ifndef SIMPLE_3D_RENDER_STATE_H
#define SIMPLE_3D_RENDER_STATE_H

#include "render_state_interface.h"
#include "util/math/color.h"

struct grSimple3DRenderStateBaseInterface:public grRenderState
{
	enum PolygonCullMode { PCM_NONE = 0, PCM_CLOCKWISE, PCM_COUNETRCLOCKWISE };

	color4          mAmbientColor;
	PolygonCullMode mPolygonCullMode;

//functions
	grSimple3DRenderStateBaseInterface();
	grSimple3DRenderStateBaseInterface(grRenderBase* render);
	~grSimple3DRenderStateBaseInterface();

	virtual void begin() {}
	virtual void finish() {}
};

#endif //SIMPLE_3D_RENDER_STATE_H