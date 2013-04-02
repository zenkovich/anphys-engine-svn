#ifndef SIMPLE_3D_RENDER_STATE_D3D8_H
#define SIMPLE_3D_RENDER_STATE_D3D8_H

#include "dependencies/dx8/include/d3d8.h"
#include "dependencies/dx8/include/d3dx8.h"
#include "dependencies/dx8/include/d3dx8core.h"

#include <windows.h>

#include "render/render_state/simple_3d_render_state_base_interface.h"

struct grSimple3DRenderStateBase:public grSimple3DRenderStateBaseInterface
{
	grSimple3DRenderStateBase():grSimple3DRenderStateBaseInterface(0) {}
	grSimple3DRenderStateBase(grRenderBase* render):grSimple3DRenderStateBaseInterface(render) {}
	virtual ~grSimple3DRenderStateBase() {}

	void begin();

	void updateTransformations();

	void bindCamera(grCamera3D* camera);
};

#endif //SIMPLE_3D_RENDER_STATE_D3D8_H