#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "surface_material_d3d8.h"
#include "render/render.h"
#include "render/material_manager/material.h"
#include "render/texture_manager/texture.h"
#include "render/surface/surface_material_manager.h"

void grSurfaceMaterialBase::setup()
{
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetMaterial(mMaterial->mMaterial);
	
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTexture(0, mTextures[0]->mTexturePtr);

	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	mSurfaceMaterialManager->mRender->m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
}

#endif //render d3d8