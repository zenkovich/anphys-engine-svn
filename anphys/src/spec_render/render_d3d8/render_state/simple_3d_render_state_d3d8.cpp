#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "simple_3d_render_state_d3d8.h"

#include "render/render.h"
#include "render/camera/camera_3d.h"
#include "render/render_target/render_target_interface.h"

void grSimple3DRenderStateBase::begin()
{
	DWORD polygonCullMode = D3DCULL_NONE;
	if (mPolygonCullMode == PCM_CLOCKWISE) polygonCullMode = D3DCULL_CW;
	else                                   polygonCullMode = D3DCULL_CCW;

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	

	
	//mRender->m_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, mAmbientColor.dwordARGB());

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//camera
	updateTransformations();
}

void grSimple3DRenderStateBase::bindCamera( grCamera3D* camera )
{
	grSimple3DRenderStateBaseInterface::bindCamera(camera);

	updateTransformations();
}

void grSimple3DRenderStateBase::updateTransformations()
{
	if (mCamera && mRender->getCurrentRenderTarget())
	{
		D3DXMATRIX projectMatrix, viewMatrix;

		vec2 renderTargetSize = mRender->getCurrentRenderTarget()->mSize;
		vec3 cameraPos = mCamera->mPosition;
		vec3 lookPoint = mCamera->mLookPoint;
		vec3 upVector = vec3(0, 1, 0);
		upVector.RotateZ(rad(mCamera->mAngle));

		D3DXMatrixPerspectiveFovLH(&projectMatrix, mCamera->mFov, renderTargetSize.x/renderTargetSize.y, 0.1f, 100000.0f);
		D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(cameraPos.x, cameraPos.y, cameraPos.z),
			&D3DXVECTOR3(lookPoint.x, lookPoint.y, lookPoint.z), 
			&D3DXVECTOR3(upVector.x, upVector.y, upVector.z) );

		mRender->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectMatrix);
		mRender->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
	}
}

#endif //RENDER_D3D8
