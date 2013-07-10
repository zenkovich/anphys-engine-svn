#include "render_system.h"

#include <string>
#include <algorithm>

#include "sprite.h"

RenderSystem::RenderSystem():mDirect3D(NULL), mDirect3DDevice(NULL), mVertexData(NULL),
	mIndexBuffer(NULL), mVertexBuffer(NULL), mLastDrawTexture(NULL), 
	mTrianglesCount(0), mFrameTrianglesCount(0), mReady(false)
{
}

RenderSystem::~RenderSystem()
{
	if (!mReady)
		return;

	removeAlltextures();

	mDirect3DDevice->Release();
	mDirect3D->Release();
	mVertexBuffer->Release();
}

bool RenderSystem::initialize( HWND hwnd, const vec2i& resolution )
{
	mReady = false;

	mResolution = resolution;

//initializing d3d8 render
	mDirect3D = Direct3DCreate8(D3D_SDK_VERSION);
	if (!mDirect3D)
	{
		printf("ERROR: Direct3DCreate8 failed!\n");
		return false;
	}

	D3DDISPLAYMODE Display;
	if(FAILED(mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		printf("ERROR: GetAdapterDisplayMode failed\n");
		return false;
	}

	ZeroMemory(&mDirect3DParametr, sizeof(mDirect3DParametr));
	mDirect3DParametr.Windowed=true;
	mDirect3DParametr.SwapEffect = D3DSWAPEFFECT_COPY;
	mDirect3DParametr.BackBufferFormat = Display.Format;
	mDirect3DParametr.BackBufferCount = 1;
	mDirect3DParametr.BackBufferWidth = (unsigned int)(resolution.x);
	mDirect3DParametr.BackBufferHeight = (unsigned int)(resolution.y);  
	mDirect3DParametr.EnableAutoDepthStencil = true;
	mDirect3DParametr.AutoDepthStencilFormat = D3DFMT_D16;

	if(FAILED(mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, 
		      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mDirect3DParametr, &mDirect3DDevice)))
	{
		printf("ERROR: CreateDevice failed\n");
		return false;
	}

//render states
	mDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	mDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	
    mDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
    mDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00000000);	
    mDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//texture state
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

	printf("Direct3d8 render initialized\n");

//create d3d vertex buffer
	if(FAILED(mDirect3DDevice->CreateVertexBuffer(nVertexBufferSize*sizeof(vertex2),
		D3DUSAGE_DYNAMIC, D3DFVF_VERTEX_2D, D3DPOOL_DEFAULT, &mVertexBuffer)))
	{
		printf("ERROR: Failed to create Direct3D8 Vertex Buffer\n");
		return false;
	}

//create d3d index buffer
	if (FAILED(mDirect3DDevice->CreateIndexBuffer(nVertexBufferSize*sizeof(WORD)/4*6, D3DUSAGE_DYNAMIC, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer)))
	{
		printf("ERROR: Failed to create Direct3D8 index buffer");
		return false;
	}

//fill index buffer
	WORD* indexData = NULL;
	if (FAILED(mIndexBuffer->Lock(0, 0, (BYTE**)&indexData, 0)))
	{
		printf("ERROR: failed to lock d3d index buffer\n");
		return false;
	}

//quad format
//
//   0------1
//   |\     |
//   |  \   |
//   |    \ |
//   3------2
// 
	for (int i = 0; i < nVertexBufferSize/4; i++)
	{
		indexData[i*6]     = i*4;
		indexData[i*6 + 1] = i*4 + 1;
		indexData[i*6 + 2] = i*4 + 2;
		
		indexData[i*6 + 3] = i*4;
		indexData[i*6 + 4] = i*4 + 2;
		indexData[i*6 + 5] = i*4 + 3;
	}

	if (FAILED(mIndexBuffer->Unlock()))
	{
		printf("ERROR: Failed to unlock d3d index buffer\n");
		return false;
	}

//setup d3d buffers
	mDirect3DDevice->SetVertexShader(D3DFVF_VERTEX_2D);
	mDirect3DDevice->SetStreamSource(0, mVertexBuffer, sizeof(vertex2));
	mDirect3DDevice->SetIndices(mIndexBuffer, 0);

//setup 2d orto matricies
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX invMatrix, translMatrix, orthoMatrix;

	D3DXMatrixScaling(&invMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&translMatrix, 0, (float)mResolution.y, 0.0f);
	D3DXMatrixOrthoOffCenterLH(&orthoMatrix, 0, (float)mResolution.x, 0, (float)mResolution.y, 0.0f, 128.0f);

	projectionMatrix = invMatrix*translMatrix*orthoMatrix;

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	mDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	mDirect3DDevice->SetTransform(D3DTS_WORLD, &identity);
	mDirect3DDevice->SetTransform(D3DTS_VIEW, &identity);

	mReady = true;

	return true;
}

bool RenderSystem::beginRender()
{
	if (!mReady)
		return false;

	if (FAILED(mDirect3DDevice->BeginScene()))
	{
		printf("ERROR: Failed BeginScene\n");
		return false;
	}

	mDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0F, 0);

//reset batching params
	mLastDrawTexture     = NULL;
	mLastDrawVertex      = NULL;
	mTrianglesCount      = 0;
	mFrameTrianglesCount = 0;
	mDIPCount            = 0;

	lockBuffer();

	return true;
}

bool RenderSystem::endRender()
{
	if (!mReady)
		return false;

//flush geometry
	unlockBuffer();
	drawPrimitives();

	if (FAILED(mDirect3DDevice->EndScene()))
	{
		printf("ERROR: Failed EndScene\n");
		return false;
	}

	mDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	//printf("Triangles: %i, DIP: %i\n", mFrameTrianglesCount, mDIPCount);

	return true;
}

Texture* RenderSystem::createTexture( const char* fileName )
{
	if (!mReady)
		return NULL;

//try get texture
	Texture* texture = getTexture(fileName);
	if (texture)
	{
		texture->incRefCount();
		return texture;
	}

//create new
	texture = new Texture;
	if (!texture->load(this, fileName))
	{
		return NULL;
	}

	mTextures.push_back(texture);

	return texture;
}

Texture* RenderSystem::getTexture( const char* fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (strcmp(fileName, (*it)->getFileName()) == 0)
			return *it;
	}

	return NULL;
}

void RenderSystem::releaseTexture( Texture* texture )
{
	texture->decRefCount();
	if (texture->getRefCount() == 0)
	{
		TexturesVec::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);
		if (fnd != mTextures.end())
			mTextures.erase(fnd);

		delete texture;
	}
}

void RenderSystem::removeAlltextures()
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		delete *it;
	}

	mTextures.clear();
}

void RenderSystem::drawSprite( Sprite* sprite )
{
	if (!mReady)
		return;

//sprite - just one quad with 4 vertex
	int spriteVertexCount = 4;

//check difference
	if (mLastDrawTexture != sprite->mTexture || 
		mLastDrawVertex + spriteVertexCount >= nVertexBufferSize)
	{
		unlockBuffer();
		drawPrimitives();
		lockBuffer();

		mLastDrawTexture = sprite->mTexture;
		if (mLastDrawTexture) mDirect3DDevice->SetTexture(0, mLastDrawTexture->mTexturePtr);
		else                  mDirect3DDevice->SetTexture(0, NULL);
	}

//copy data
	//memcpy(mVertexData + mLastDrawingVertex*sizeof(vertex2d), mesh->mVertexBuffer, sizeof(vertex2d)*mesh->mVertexCount);
	for (int i = 0; i < spriteVertexCount; i++)
	{
		mVertexData[i + mLastDrawVertex] = sprite->mVerticies[i];
	}

	mTrianglesCount += 2;
	mLastDrawVertex += spriteVertexCount;
}

void RenderSystem::lockBuffer()
{
	if (FAILED(mVertexBuffer->Lock(0, 0, (BYTE**)&mVertexData, D3DLOCK_DISCARD)))
	{
		printf("ERROR: Failed to lock d3d vertex buffer\n");
		return;
	}

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = 0;
}

void RenderSystem::unlockBuffer()
{
	if (FAILED(mVertexBuffer->Unlock()))
	{
		printf("ERROR: failed to nlock d3d vertex buffer\n");
	}
}

void RenderSystem::drawPrimitives()
{
	if (mLastDrawVertex < 1)
		return;

	if (FAILED(mDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, mLastDrawVertex, 0, mTrianglesCount)))
	{
		printf("ERROR: Failed call DrawIndexedPrimitive\n");
	}

	mDIPCount++;
}

Texture::Texture():mTexturePtr(NULL), mRefCount(0), mRenderSystem(NULL)
{
	mFilename[0] = '\0';
}

Texture::Texture( RenderSystem* renderSystem, const char* fileName ):mTexturePtr(NULL), mRefCount(0), mRenderSystem(NULL)
{
	mFilename[0] = '\0';
	load(renderSystem, fileName);
}

Texture::~Texture()
{
	if (mTexturePtr)
		mTexturePtr->Release();
}

bool Texture::load( RenderSystem* renderSystem, const char* fileName )
{
	strcpy(mFilename, fileName);
	mRenderSystem = renderSystem;
	mRefCount = 0;
	
	if (FAILED(D3DXCreateTextureFromFile(mRenderSystem->getDirect3DDevice(), fileName, &mTexturePtr)))
	{
		printf("ERROR: Failed to load texture '%s'\n", fileName);
		return false;
	}

	D3DSURFACE_DESC desc;
	mTexturePtr->GetLevelDesc(0, &desc);
	mSize = vec2f((float)desc.Width, (float)desc.Height);

	return true;
}

void Texture::incRefCount()
{
	mRefCount++;
}

void Texture::decRefCount()
{
	mRefCount--;
}

int Texture::getRefCount()
{
	return mRefCount;
}

vec2f Texture::getSize() const
{
	return mSize;
}

char* const Texture::getFileName()
{
	return mFilename;
}
