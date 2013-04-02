#ifndef RENDER_STATE_INTERFACE_H
#define RENDER_STATE_INTERFACE_H

#include "util/other/type_intexation.h"

struct grRenderBase;

struct grRenderState
{
	DEFINE_TYPE(grRenderState)

	enum PolygonCullMode { PCM_NONE = 0, PCM_CLOCKWISE, PCM_COUNETRCLOCKWISE };

	grRenderBase* mRender;

//functions
	grRenderState():mRender(0) {}
	grRenderState(grRenderBase* render):mRender(render) {}
	virtual ~grRenderState() {}

	virtual void begin() {}
	virtual void finish() {}
	virtual void updateTransformations() {}
	virtual void flush() {}
};

#endif //RENDER_STATE_INTERFACE_H