#ifndef CAMERAD3D8_H
#define CAMERAD3D8_H

#include "../../util/math/mmath.h"
#include "../camera.h"
#include "render_d3d8.h"

struct grCameraD3D:public grCamera
{	
	D3DXMATRIX mViewMatrix;
	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mProjectMatrix;

	grCameraD3D() {}
	grCameraD3D(vec2 screenSize, grRenderBase* render) 
	{
		initialize(screenSize, render);
	}

	virtual void initialize(vec2 screenSize, grRenderBase* render) 
	{ 
		mScreenSize = screenSize; 
		mRenderBase = render; 
	}

	virtual void update(float dt)
	{
		mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &mWorldMatrix);
		mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &mViewMatrix);
		mRenderBase->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &mProjectMatrix);		
	}
};

#endif //CAMERAD3D8_H