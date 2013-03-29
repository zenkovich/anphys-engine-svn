#ifndef RENDER_TARGET_INTERFACE_H
#define RENDER_TARGET_INTERFACE_H

#include "util/other/type_intexation.h"

struct grRenderBase;

struct grRenderTarget
{
	DEFINE_TYPE(grRenderTarget)

	grRenderBase* mRender;

//functions
	grRenderTarget():mRender() {}
	grRenderTarget(grRenderBase* render):mRender(render) {}
	virtual ~grRenderTarget() {}

	virtual bool setup() { return true; }
	virtual bool complete() { return true; }
};

#endif //RENDER_TARGET_INTERFACE_H