#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "backbuffer_render_target_d3d8.h"
#include "render/render.h"

grBackbufferRenderTargetBase::grBackbufferRenderTargetBase():mRenderTargetSurface(NULL), mDepthStencilSurface(NULL)
{
}

grBackbufferRenderTargetBase::grBackbufferRenderTargetBase( grRenderBase* render ):grBackbufferRenderTargetBaseInterface(render)
{
	if (FAILED(mRender->m_pDirect3DDevice->GetRenderTarget(&mRenderTargetSurface)))
		render->mLog->fout(1, "ERROR: Can't initialize backbuffer render target: GetRenderTarget failed");

	if (FAILED(mRender->m_pDirect3DDevice->GetDepthStencilSurface(&mDepthStencilSurface)))
		render->mLog->fout(1, "ERROR: Can't initialize backbuffer render target: GetDepthStencilSurface failed");
}

bool grBackbufferRenderTargetBase::begin()
{
	return !(FAILED(mRender->m_pDirect3DDevice->SetRenderTarget(mRenderTargetSurface, mDepthStencilSurface)));
}

#endif //RENDER_D3D8
