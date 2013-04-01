#ifndef RENDER_TARGET_INTERFACE_H
#define RENDER_TARGET_INTERFACE_H

#include "util/other/type_intexation.h"
#include "util/math/mmath.h"

struct grRenderBase;

struct grRenderTarget
{
	DEFINE_TYPE(grRenderTarget)

	grRenderBase* mRender;
	vec2          mSize;

//functions
	grRenderTarget():mRender() {}
	grRenderTarget(grRenderBase* render):mRender(render) {}
	virtual ~grRenderTarget() {}

	virtual bool begin() { return true; }
	virtual bool finish() { return true; }
};

#endif //RENDER_TARGET_INTERFACE_H