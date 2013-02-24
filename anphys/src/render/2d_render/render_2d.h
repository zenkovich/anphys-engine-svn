#ifndef RENDER_2D_H
#define RENDER_2D_H

#include "engine/engine_options.h"
#include "util/math/mmath.h"

#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/2d_render/render_2d_d3d8.h"
#endif //RENDER_D3D8

struct grRender2D:public grRender2DBase
{
	grRender2D(grRenderBaseInterface* render):grRender2DBase(render) {}
	~grRender2D() {}
};

#endif //RENDER_2D_H