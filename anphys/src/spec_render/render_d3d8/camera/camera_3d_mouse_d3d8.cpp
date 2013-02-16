#include "../../engine/engine_options.h"

#ifdef RENDER_D3D8
#include "../camera_3d_mouse.h"
#include "render_d3d8.h"

#include <d3d8.h>
#include <d3dx8.h>
#include <d3dx8core.h>

void grCamera3DMouse::specRenderUpdate(float dt)
{
	D3DXMATRIX mViewMatrix, mWorldMatrix, mProjectMatrix;

	D3DXMatrixLookAtLH(&mViewMatrix, &D3DXVECTOR3(mPosition.x,mPosition.y,mPosition.z), &D3DXVECTOR3(mLookPoint.x,mLookPoint.y,mLookPoint.z), &D3DXVECTOR3(0,1,0));
	D3DXMatrixRotationX(&mWorldMatrix, 0);
	D3DXMatrixPerspectiveFovLH(&mProjectMatrix, D3DX_PI/4.0f, (float)mScreenSize.x/(float)mScreenSize.y, 0.1f, 1000000.0f);

	mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &mWorldMatrix);
	mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &mViewMatrix);
	mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &mProjectMatrix);
}

#endif