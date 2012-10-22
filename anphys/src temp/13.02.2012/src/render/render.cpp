#include "render.h"

#ifdef RENDER_D3D8
#include "../engine/render_system/render_frame_win32_window/render_frame_win32_window.h"
#endif //RENDER_D3D8

grRender::grRender():grRenderBase(), mFrame(NULL) {}

grRender::grRender(grRenderFrame* frame, fRect outputRect)
{
	mFrame = frame;
#ifdef RENDER_D3D8

	grRenderBase::initialize(static_cast<apRenderWindow*>(frame)->mHWnd, outputRect);

#endif //RENDER_D3D8
}

grRender::~grRender() {}