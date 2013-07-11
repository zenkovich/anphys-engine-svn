#include "render_system.h"

#include <string>
#include <algorithm>

#include "sprite.h"

RenderSystem::RenderSystem():mVertexData(NULL),mLastDrawTexture(NULL), 
	mTrianglesCount(0), mFrameTrianglesCount(0), mReady(false)
{
}

RenderSystem::~RenderSystem()
{
	if (!mReady)
		return;

	if (mGLContext)										
	{
		if (!wglMakeCurrent(NULL,NULL))				
		{
			printf("ERROR: Release Of DC And RC Failed.\n");
		}

		if (!wglDeleteContext(mGLContext))					
		{
			printf("ERROR: Release Rendering Context Failed.\n");
		}

		mGLContext = NULL;				
	}

	removeAlltextures();
}

void ortho(float* mat, float left, float right, float bottom, float top, float nearz, float farz) {
	float tx = -(right + left)/(right - left);
	float ty = -(top + bottom)/(top - bottom);
	float tz = -(farz + nearz)/(farz - nearz);
		
	mat[0] = 2.0f/(right - left); mat[4] = 0.0f; mat[8] = 0.0f; mat[12] = tx;
	mat[1] = 0.0f; mat[5] = 2.0f/(top - bottom); mat[9] = 0.0f; mat[13] = ty;
	mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = -2.0f/(farz - nearz); mat[14] = tz;
	mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
}	

bool RenderSystem::initialize( HWND hwnd, const vec2i& resolution )
{
	mReady = false;

	mResolution = resolution;

	GLuint pixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,  										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	mHWndDC = GetDC(hwnd);
	if (!mHWndDC)						
	{					
		printf("ERROR: Can't Create A GL Device Context.\n");
		return false;						
	}

	pixelFormat = ChoosePixelFormat(mHWndDC, &pfd);
	if (!pixelFormat)	
	{	
		printf("ERROR: Can't Find A Suitable PixelFormat.\n");
		return false;								
	}

	if (!SetPixelFormat(mHWndDC, pixelFormat, &pfd))	
	{
		printf("ERROR: Can't Set The PixelFormat.\n");
		return false;
	}

	mGLContext = wglCreateContext(mHWndDC);
	if (!mGLContext)	
	{
		printf("ERROR: Can't Create A GL Rendering Context.\n");
		return false;
	}

	if(!wglMakeCurrent(mHWndDC, mGLContext))			
	{
		printf("ERROR: Can't Activate The GL Rendering Context.\n");
		return false;
	}

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing

	mVertexData = new unsigned char[nVertexBufferSize*sizeof(vertex2)];

	const unsigned int triesMaxCount = nVertexBufferSize*6/4;
	mVertexIndexData = new unsigned short[triesMaxCount];
	mLastDrawVertex = 0;
	mTrianglesCount = 0;

	// заполняет индексный буффер для всего вертекс буффера
	
	for(int i = 0, n = 0; i < nVertexBufferSize/4; ++i)
	{	
		mVertexIndexData[i*6 + 0] = n;
		mVertexIndexData[i*6 + 1] = n + 1;
		mVertexIndexData[i*6 + 2] = n + 2;

		mVertexIndexData[i*6 + 3] = n + 0;
		mVertexIndexData[i*6 + 4] = n + 2;
		mVertexIndexData[i*6 + 5] = n + 3;
		
		n+=4;
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex2), mVertexData + sizeof(float)*3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex2), mVertexData + sizeof(float)*3 + sizeof(unsigned long));
	glVertexPointer(3, GL_FLOAT, sizeof(vertex2), mVertexData + 0);

	float projMat[16];
	ortho(projMat, 0.0f, (float)mResolution.x, (float)mResolution.y, 0.0f, 0.0f, 10.0f);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, mResolution.x, mResolution.y);
	glLoadMatrixf(projMat);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);

	mReady = true;

	return true;
}

bool RenderSystem::beginRender()
{
	if (!mReady)
		return false;

//reset batching params
	mLastDrawTexture     = NULL;
	mLastDrawVertex      = NULL;
	mTrianglesCount      = 0;
	mFrameTrianglesCount = 0;
	mDIPCount            = 0;

	lockBuffer();
	
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	return true;
}

bool RenderSystem::endRender()
{
	if (!mReady)
		return false;

//flush geometry
	unlockBuffer();
	drawPrimitives();

	SwapBuffers(mHWndDC);

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
		if (mLastDrawTexture) 
		{			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);
		}
		else          
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

//copy data
	memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], sprite->mVerticies, sizeof(vertex2)*4);
	/*for (int i = 0; i < spriteVertexCount; i++)
	{
		mVertexData[i + mLastDrawVertex] = sprite->mVerticies[i];
	}*/

	mTrianglesCount += 2;
	mLastDrawVertex += spriteVertexCount;
}

void RenderSystem::lockBuffer()
{
	/*if (FAILED(mVertexBuffer->Lock(0, 0, (BYTE**)&mVertexData, D3DLOCK_DISCARD)))
	{
		printf("ERROR: Failed to lock d3d vertex buffer\n");
		return;
	}*/

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = 0;
}

void RenderSystem::unlockBuffer()
{
	/*if (FAILED(mVertexBuffer->Unlock()))
	{
		printf("ERROR: failed to nlock d3d vertex buffer\n");
	}*/
}

void RenderSystem::drawPrimitives()
{
	if (mLastDrawVertex < 1)
		return;

	glDrawElements(GL_TRIANGLES, mTrianglesCount, GL_UNSIGNED_SHORT, mVertexIndexData);
	/*if (FAILED(mDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, mLastDrawVertex, 0, mTrianglesCount)))
	{
		printf("ERROR: Failed call DrawIndexedPrimitive\n");
	}*/

	mDIPCount++;
}

Texture::Texture():mHandle(NULL), mRefCount(0), mRenderSystem(NULL)
{
	mFilename[0] = '\0';
}

Texture::Texture( RenderSystem* renderSystem, const char* fileName ):mHandle(NULL), mRefCount(0), mRenderSystem(NULL)
{
	mFilename[0] = '\0';
	load(renderSystem, fileName);
}

Texture::~Texture()
{
	/*if (mHandle)
		mTexturePtr->Release();*/
}

bool Texture::load( RenderSystem* renderSystem, const char* fileName )
{
	strcpy(mFilename, fileName);
	mRenderSystem = renderSystem;
	mRefCount = 0;
	
	/*if (FAILED(D3DXCreateTextureFromFile(mRenderSystem->getDirect3DDevice(), fileName, &mTexturePtr)))
	{
		printf("ERROR: Failed to load texture '%s'\n", fileName);
		return false;
	}

	D3DSURFACE_DESC desc;
	mTexturePtr->GetLevelDesc(0, &desc);
	mSize = vec2f((float)desc.Width, (float)desc.Height);*/

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
