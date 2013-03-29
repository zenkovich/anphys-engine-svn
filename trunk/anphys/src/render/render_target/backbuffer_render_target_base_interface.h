#ifndef BACKBUFFER_RENDER_TARGET_BASE_INTERFACE_H
#define BACKBUFFER_RENDER_TARGET_BASE_INTERFACE_H

#include "render_target_interface.h"

struct grBackbufferRenderTargetBaseInterface:public grRenderTarget
{
	grBackbufferRenderTargetBaseInterface():grRenderTarget() {}
	grBackbufferRenderTargetBaseInterface(grRenderBase* render):grRenderTarget(render) {}
};

#endif //BACKBUFFER_RENDER_TARGET_BASE_INTERFACE_H