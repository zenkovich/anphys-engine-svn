#ifndef RENDER_H
#define RENDER_H

#include "../engine/engine_options.h"
#include "../util/math/mmath.h"

#ifdef RENDER_D3D8
#include "d3d8/render_d3d8.h"
#endif //RENDER_D3D8

struct grRenderFrame;
struct cRenderStuff;

struct grRender:public grRenderBase
{
	grRenderFrame* mFrame;
	cRenderStuff*  mRenderStuff;


	grRender();
	grRender(grRenderFrame* frame, fRect outputRect);

	virtual ~grRender();

	
	void createStdMaterials();
};


#endif //RENDER_H