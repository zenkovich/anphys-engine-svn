#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "simple_3d_render_state_d3d8.h"

#include "render/render.h"

void grSimple3DRenderStateBase::begin()
{
	DWORD polygonCullMode = D3DCULL_NONE;
	if (mPolygonCullMode == PCM_CLOCKWISE) polygonCullMode = D3DCULL_CW;
	else                                   polygonCullMode = D3DCULL_CCW;

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, polygonCullMode);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, mAmbientColor.dwordARGB());

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

#endif //RENDER_D3D8
