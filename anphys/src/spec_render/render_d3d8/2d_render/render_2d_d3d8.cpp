#include "render_2d_d3d8.h"

//framesystem
#include "engine/frame_system/render_frame.h"

//render
#include "render/render.h"
#include "spec_render/render_d3d8/objects/2d/render_2d_object_mesh_d3d8.h"
#include "render/texture_manager/texture.h"
#include "render/camera/camera_2d.h"

//util
#include "util/math/mmath.h"


grRender2DBase::grRender2DBase( grRenderBaseInterface* render ):grRender2DBaseInterface(render)
{
	mLastDrawingIndex = mLastDrawingVertex = mTrianglesCount = 0;
	mLastDrawingTexture = NULL;

	mVertexBuffer = NULL;
	mIndexBuffer = NULL;
}

grRender2DBase::~grRender2DBase()
{
	if (mVertexBuffer) mVertexBuffer->Release();
	if (mIndexBuffer) mIndexBuffer->Release();
}

void grRender2DBase::beginRender()
{
	if (!mReady || mRendering) 
		return;

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	setupMatrix();

	mLastDrawingTexture = NULL;	
	mFrameTrianglesCount = 0;

	lockBuffers();

	mRendering = true;
}

void grRender2DBase::endRender()
{
	if (!mRendering)
		return;

	unlockBuffers();
	drawPrimitives();

	revertMatrix();

	incFrameIdx();

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	mRendering = false;
}

void grRender2DBase::drawPrimitives()
{
	mRender->m_pDirect3DDevice->SetVertexShader(D3DFVF_VERTEX_2D);
	mRender->m_pDirect3DDevice->SetStreamSource(0, mVertexBuffer, sizeof(vertex2d));
	mRender->m_pDirect3DDevice->SetIndices(mIndexBuffer, 0);
	mRender->m_pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, mLastDrawingVertex, 0, mTrianglesCount);
}

void grRender2DBase::drawMesh( grRender2DObjectMeshBase* renderMesh )
{
	bool diffTexture = renderMesh->mTextures.empty() || renderMesh->mTextures[0] != mLastDrawingTexture;

	if (diffTexture || 
		mLastDrawingVertex + renderMesh->mVertexCount >= nVertexBufferSize ||
		mLastDrawingIndex + renderMesh->mPolygonsCount*3 >= nIndexBufferSize)
	{
		unlockBuffers();
		drawPrimitives();
		lockBuffers();

		if (diffTexture) 
		{
			if (renderMesh->mTextures.empty()) mLastDrawingTexture = NULL;
			else                               setTexture(renderMesh->mTextures[0]);
		}
	}

	memcpy(mVertexData, renderMesh->mVertexBuffer, sizeof(vertex2d)*renderMesh->mVertexCount);

	for (unsigned int i = 0; i < renderMesh->mPolygonsCount; i++)
	{
		mIndexData[mLastDrawingIndex++] = renderMesh->mPolygonsBuffer[i].a + mLastDrawingVertex;
		mIndexData[mLastDrawingIndex++] = renderMesh->mPolygonsBuffer[i].b + mLastDrawingVertex;
		mIndexData[mLastDrawingIndex++] = renderMesh->mPolygonsBuffer[i].c + mLastDrawingVertex;
		mTrianglesCount++;
	}

	mLastDrawingVertex += renderMesh->mVertexCount;

	//for (unsigned int i = 0; i < renderMesh)
}

void grRender2DBase::updateMatrix()
{
	D3DXMATRIX tmp, matView;
	D3DXMATRIX m0, m00, m1, m2, m3, m4;
	
	const float halfwidth  = mCamera->mScreenSize.x*0.5f;
	const float halfheight = mCamera->mScreenSize.y*0.5f;

	float camx = mCamera->mPosition.x;
	float camy = mCamera->mPosition.y;

	D3DXMatrixScaling(&m0, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&m1, camx - halfwidth, mCamera->mScreenSize.y - camy - halfheight, 0.0f);
	D3DXMatrixScaling(&m2, mCamera->mScale.x, mCamera->mScale.y, 1.0f);
	D3DXMatrixRotationZ(&m3, rad(mCamera->mAngle));
	D3DXMatrixTranslation(&m4, halfwidth - 0.5f, halfheight + 0.5f, 0.0f);

	D3DXMatrixIdentity(&mProjMatrix);
	mProjMatrix = m0*m1*m2*m3*m4;

	D3DXMatrixOrthoOffCenterLH(&tmp, 0, mCamera->mScreenSize.x, 0, mCamera->mScreenSize.y, 0.0f, 128.0f);
	D3DXMatrixMultiply(&mProjMatrix, &mProjMatrix, &tmp);
	D3DXMatrixIdentity(&mViewMatrix);
}

void grRender2DBase::setupMatrix()
{
	mRender->m_pDirect3DDevice->GetTransform(D3DTS_PROJECTION, &mLastProjMatrix);
	mRender->m_pDirect3DDevice->GetTransform(D3DTS_VIEW, &mLastViewMatrix);

	mRender->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &mProjMatrix);
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &mViewMatrix);
	D3DXMATRIX tt;

	D3DXMatrixIdentity(&tt);
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &tt);
}

void grRender2DBase::revertMatrix()
{
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &mLastProjMatrix);
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &mLastViewMatrix);
}

void grRender2DBase::lockBuffers()
{
	if (FAILED(mVertexBuffer->Lock(0, 0, (BYTE**)&mVertexData, D3DLOCK_DISCARD)))
	{
		assert(0, "Failed to lock Direct3D8 vertex buffer");
		return;
	}

	if (FAILED(mIndexBuffer->Lock(0, 0, (BYTE**)&mIndexData, D3DLOCK_DISCARD)))
	{
		assert(0, "Failed to lock Direct3D8 index buffer");
		return;
	}

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawingIndex = mLastDrawingVertex = mTrianglesCount = 0;
}

void grRender2DBase::unlockBuffers()
{
	if (FAILED(mVertexBuffer->Unlock()))
	{
		assert(0, "Failed to unlock Direct3D8 vertex buffer");
		return;
	}

	if (FAILED(mIndexBuffer->Unlock()))
	{
		assert(0, "Failed to unlock Direct3D8 index buffer");
		return;
	}
}

void grRender2DBase::setTexture( grTexture* texture )
{
	mRender->m_pDirect3DDevice->SetTexture(0, texture->mTexturePtr);
}

void grRender2DBase::initialize()
{
	//create vertex buffer	
	if(FAILED(mRender->m_pDirect3DDevice->CreateVertexBuffer(nVertexBufferSize*sizeof(vertex2d),
		D3DUSAGE_DYNAMIC, D3DFVF_VERTEX_2D, D3DPOOL_DEFAULT, &mVertexBuffer)))
	{
		assert(0, "Failed to create Direct3D8 Vertex Buffer");
		return;
	}

	//create index buffer
	if (FAILED(mRender->m_pDirect3DDevice->CreateIndexBuffer(nIndexBufferSize*sizeof(WORD), D3DUSAGE_WRITEONLY, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer)))
	{
		assert(0, "Failed to create Direct3D8 index buffer");
		return;
	}

	mCamera = new grCamera2D(mRender->mFrame->mInRect.getSize(), mRender);

	updateMatrix();

	mFrameIdx = 0;
	mFrameTrianglesCount = 0;

	mReady = true;
}
