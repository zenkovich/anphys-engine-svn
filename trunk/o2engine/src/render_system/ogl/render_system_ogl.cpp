#include "render_system_ogl.h"

#include "../mesh.h"
#include "../texture.h"
#include "app/application.h"
#include "util/log/log_stream.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

grRenderSystem::grRenderSystem( cApplication* application ):
	grRenderSystemBaseInterface(application), mReady(false)
{
	initializeGL();
}

grRenderSystem::~grRenderSystem()
{
	deinitializeGL();
}

bool grRenderSystem::beginRender()
{
	if (!mReady)
		return false;

//reset batching params
	mLastDrawTexture     = NULL;
	mLastDrawVertex      = NULL;
	mTrianglesCount      = 0;
	mFrameTrianglesCount = 0;
	mDIPCount            = 0;
			
	float projMat[16];
	orthoProjMatrix(projMat, 0.0f, (float)mResolution.x, (float)mResolution.y, 0.0f, 0.0f, 10.0f);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, mResolution.x, mResolution.y);
	glLoadMatrixf(projMat);

	updateCameraTransforms();
	
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	return true;
}

bool grRenderSystem::endRender()
{
	if (!mReady)
		return false;

//flush geometry
	drawPrimitives();

	SwapBuffers(mHDC);

	return true;
}

bool grRenderSystem::drawMesh( grMesh* mesh )
{
	if (!mReady)
		return false;

//check difference
	if (mLastDrawTexture != mesh->mTexture || 
		mLastDrawVertex + mesh->mVertexCount >= nVertexBufferSize ||
		mLastDrawIdx + mesh->mPolyCount*3 >= nIndexBufferSize)
	{
		drawPrimitives();

		mLastDrawTexture = mesh->mTexture;
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
	memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], mesh->mVerticies, sizeof(vertex2)*mesh->mVertexCount);

	for (unsigned int i = mLastDrawIdx, j = 0; i < mesh->mPolyCount*3; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + mesh->mIndexes[j];
	}

	mTrianglesCount += 2;
	mLastDrawVertex += mesh->mVertexCount;
	mLastDrawIdx += mesh->mPolyCount*3;

	return true;
}

void grRenderSystem::updateCameraTransforms()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-1.0f);	
}

void grRenderSystem::initializeGL()
{
	mLog->out("Initializing OpenGL render..");

	mApplication->getOption(cApplicationOption::CLIENT_RECT, &mResolution);

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

	mHDC = GetDC(mApplication->mHWnd);
	if (!mHDC)						
	{					
		mLog->out("ERROR: Can't Create A GL Device Context.\n");
		return;						
	}

	pixelFormat = ChoosePixelFormat(mHDC, &pfd);
	if (!pixelFormat)	
	{	
		mLog->out("ERROR: Can't Find A Suitable PixelFormat.\n");
		return;								
	}

	if (!SetPixelFormat(mHDC, pixelFormat, &pfd))	
	{
		mLog->out("ERROR: Can't Set The PixelFormat.\n");
		return;
	}

	mGLContext = wglCreateContext(mHDC);
	if (!mGLContext)	
	{
		mLog->out("ERROR: Can't Create A GL Rendering Context.\n");
		return;
	}

	if(!wglMakeCurrent(mHDC, mGLContext))			
	{
		mLog->out("ERROR: Can't Activate The GL Rendering Context.\n");
		return;
	}

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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mReady = true;
}

void grRenderSystem::deinitializeGL()
{
	if (!mReady)
		return;

	if (mGLContext)										
	{		
		removeAllTextures();

		if (!wglMakeCurrent(NULL,NULL))				
		{
			mLog->out("ERROR: Release Of DC And RC Failed.\n");
		}

		if (!wglDeleteContext(mGLContext))					
		{
			mLog->out("ERROR: Release Rendering Context Failed.\n");
		}

		mGLContext = NULL;				
	}

	mReady = false;
}

void grRenderSystem::drawPrimitives()
{	
	if (mLastDrawVertex < 1)
		return;

	glDrawElements(GL_TRIANGLES, mTrianglesCount*3, GL_UNSIGNED_SHORT, mVertexIndexData);

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = 0;

	mDIPCount++;
}

void grRenderSystem::frameResized()
{
	mApplication->getOption(cApplicationOption::WND_SIZE, &mResolution);
}

CLOSE_O2_NAMESPACE