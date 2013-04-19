#ifndef STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H
#define STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H

#include "render_target_interface.h"

struct grStencilBufferRenderTargetBaseInterface:public grRenderTarget
{
	grStencilBufferRenderTargetBaseInterface():grRenderTarget() {}
	grStencilBufferRenderTargetBaseInterface(grRenderBase* render):grRenderTarget(render) {}
	virtual ~grStencilBufferRenderTargetBaseInterface() {}
};

#endif //STENCIL_BUFFER_RENDER_TARGET_BASE_INTERFACE_H