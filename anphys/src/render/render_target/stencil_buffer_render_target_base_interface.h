#ifndef STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H
#define STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H

#include "render_target_interface.h"
#include "util/math/mmath.h"
#include "util/math/rect.h"

struct grStencilBufferRenderTargetBaseInterface:public grRenderTarget
{
	grStencilBufferRenderTargetBaseInterface():grRenderTarget() {}
	grStencilBufferRenderTargetBaseInterface(grRenderBase* render):grRenderTarget(render) {}
	grStencilBufferRenderTargetBaseInterface(grRenderBase* render, const vec2& size):
		grRenderTarget(render) { mSize = size; }
	virtual ~grStencilBufferRenderTargetBaseInterface() {}

	virtual void clear() {}
	virtual void fillRect(const fRect& rect) {}
};

#endif //STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H