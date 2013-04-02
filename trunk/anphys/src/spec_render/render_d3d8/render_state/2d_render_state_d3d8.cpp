#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "2d_render_state_d3d8.h"

#include "render/render.h"
#include "render/texture_manager/texture.h"
#include "render/render_target/render_target_interface.h"
#include "render/camera/camera_2d.h"
#include "spec_render/render_d3d8/objects/2d/render_2d_object_mesh_d3d8.h"

gr2DRenderStateBase::gr2DRenderStateBase()
{
	mLastDrawingIndex = mLastDrawingVertex = mTrianglesCount = 0;
	mLastDrawingTexture = NULL;

	mVertexBuffer = NULL;
	mIndexBuffer = NULL;

	mRendering = false;
}

gr2DRenderStateBase::gr2DRenderStateBase( grRenderBase* render )
{
	mLastDrawingIndex = mLastDrawingVertex = mTrianglesCount = 0;
	mLastDrawingTexture = NULL;

	mVertexBuffer = NULL;
	mIndexBuffer = NULL;

	mRendering = false;

	initialize();
}

gr2DRenderStateBase::~gr2DRenderStateBase()
{
	if (mVertexBuffer) mVertexBuffer->Release();
	if (mIndexBuffer) mIndexBuffer->Release();
}

void gr2DRenderStateBase::begin()
{
	DWORD polygonCullMode = D3DCULL_NONE;
	if (mPolygonCullMode == PCM_CLOCKWISE) polygonCullMode = D3DCULL_CW;
	else                                   polygonCullMode = D3DCULL_CCW;

	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, polygonCullMode);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mRender->m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	

	setupMatrix();

	mLastDrawingTexture = NULL;	
	mFrameTrianglesCount = 0;

	lockBuffers();

	mRendering = true;
}

void gr2DRenderStateBase::finish()
{
	if (!mRendering)
		return;

	unlockBuffers();
	drawPrimitives();

	incFrameIdx();

	mRendering = false;
}

void gr2DRenderStateBase::drawMesh( grRender2DObjectMeshBase* mesh )
{
	bool diffTexture = mesh->mTextures.empty() || mesh->mTextures[0] != mLastDrawingTexture;

	if (diffTexture || 
		mLastDrawingVertex + mesh->mVertexCount >= nVertexBufferSize ||
		mLastDrawingIndex + mesh->mPolygonsCount*3 >= nIndexBufferSize)
	{
		unlockBuffers();
		drawPrimitives();
		lockBuffers();

		if (diffTexture) 
		{
			if (mesh->mTextures.empty()) mLastDrawingTexture = NULL;
			else                         setTexture(mesh->mTextures[0]);
		}
	}

	memcpy(mVertexData + mLastDrawingVertex*sizeof(vertex2d), mesh->mVertexBuffer, sizeof(vertex2d)*mesh->mVertexCount);

	for (unsigned int i = 0; i < mesh->mPolygonsCount; i++)
	{
		mIndexData[mLastDrawingIndex++] = mesh->mPolygonsBuffer[i].a + mLastDrawingVertex;
		mIndexData[mLastDrawingIndex++] = mesh->mPolygonsBuffer[i].b + mLastDrawingVertex;
		mIndexData[mLastDrawingIndex++] = mesh->mPolygonsBuffer[i].c + mLastDrawingVertex;
		mTrianglesCount++;
	}

	mLastDrawingVertex += mesh->mVertexCount;
}

void gr2DRenderStateBase::drawPrimitives()
{
	mRender->m_pDirect3DDevice->SetVertexShader(D3DFVF_VERTEX_2D);
	mRender->m_pDirect3DDevice->SetStreamSource(0, mVertexBuffer, sizeof(vertex2d));
	mRender->m_pDirect3DDevice->SetIndices(mIndexBuffer, 0);
	mRender->m_pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, mLastDrawingVertex, 0, mTrianglesCount);
}

void gr2DRenderStateBase::lockBuffers()
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

void gr2DRenderStateBase::unlockBuffers()
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

void gr2DRenderStateBase::setupMatrix()
{
	D3DXMATRIX tmp, matView;
	D3DXMATRIX m0, m00, m1, m2, m3, m4;

	vec2 screenSize = mRender->getCurrentRenderTarget()->mSize;

	const float halfwidth  = screenSize.x*0.5f;
	const float halfheight = screenSize.y*0.5f;

	float camAngle = 0;
	vec2 camPos(0, 0), camScale(1, 1);
	if (mCamera)
	{ 
		camPos = mCamera->mPosition;
		camScale = mCamera->mScale;
		camAngle = mCamera->mAngle;
	}

	D3DXMatrixScaling(&m0, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&m1, camPos.x - halfwidth, screenSize.y - camPos.y - halfheight, 0.0f);
	D3DXMatrixScaling(&m2, camScale.x, camScale.y, 1.0f);
	D3DXMatrixRotationZ(&m3, rad(camAngle));
	D3DXMatrixTranslation(&m4, halfwidth - 0.5f, halfheight + 0.5f, 0.0f);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixIdentity(&projectionMatrix);
	projectionMatrix = m0*m1*m2*m3*m4;

	D3DXMatrixOrthoOffCenterLH(&tmp, 0, screenSize.x, 0, screenSize.y, 0.0f, 128.0f);
	D3DXMatrixMultiply(&projectionMatrix, &projectionMatrix, &tmp);

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	mRender->m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &identity);
	mRender->m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &identity);
}

void gr2DRenderStateBase::setTexture( grTexture* texture )
{
	mRender->m_pDirect3DDevice->SetTexture(0, texture->mTexturePtr);
}

void gr2DRenderStateBase::initialize()
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

	mFrameIdx = 0;
	mFrameTrianglesCount = 0;
}

#endif //RENDER_D3D8