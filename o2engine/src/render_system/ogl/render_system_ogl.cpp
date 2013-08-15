#include "render_system_ogl.h"

#include "../mesh.h"
#include "../render_target.h"
#include "../texture.h"
#include "../camera.h"
#include "app/application.h"
#include "util/log/log_stream.h"
#include "util/math/math.h"

#ifdef PLATFORM_WIN
	#include "ogl_ext_win.h"
#endif // PLATFORM_WIN

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

void grRenderSystem::initializeGL()
{
	mLog->out("Initializing OpenGL render..");

	mApplication->getOption(cApplicationOption::CLIENT_RECT, &mResolution);

#ifdef PLATFORM_WIN
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

	//get gl extensions
	getGLExtensions(mLog);

#endif //PLATFORM_WIN

	mVertexData = new unsigned char[mVertexBufferSize*sizeof(vertex2)];

	mVertexIndexData = new unsigned short[mIndexBufferSize];
	mLastDrawVertex = 0;
	mTrianglesCount = 0;
	mCurrentPrimitiveType = GL_TRIANGLES;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex2), mVertexData + sizeof(float)*3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex2), mVertexData + sizeof(float)*3 + sizeof(unsigned long));
	glVertexPointer(3, GL_FLOAT, sizeof(vertex2), mVertexData + 0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(1.0f);

	mCurrentRenderTarget = NULL;

	mReady = true;
}

bool grRenderSystem::beginRender()
{
	if (!mReady)
		return false;

//reset batching params
	mLastDrawTexture      = NULL;
	mLastDrawVertex       = 0;
	mLastDrawIdx          = 0;
	mTrianglesCount       = 0;
	mFrameTrianglesCount  = 0;
	mDIPCount             = 0;
	mCurrentPrimitiveType = GL_TRIANGLES;

	setupMatrix(mResolution.castTo<float>());

	updateCameraTransforms();

	clear();

	return true;
}

void grRenderSystem::drawPrimitives()
{	
	if (mLastDrawVertex < 1)
		return;
	
	glDrawElements(mCurrentPrimitiveType, mLastDrawIdx, GL_UNSIGNED_SHORT, mVertexIndexData);

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;

	mDIPCount++;
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
		mLastDrawVertex + mesh->mVertexCount >= mVertexBufferSize ||
		mLastDrawIdx + mesh->mPolyCount*3 >= mIndexBufferSize ||
		mCurrentPrimitiveType == GL_LINES)
	{
		drawPrimitives();

		mLastDrawTexture = mesh->mTexture;
		mCurrentPrimitiveType = GL_TRIANGLES;

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
	//memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], mesh->mVerticies, sizeof(vertex2)*mesh->mVertexCount);
	for (unsigned int i = mLastDrawVertex, j = 0; j < mesh->mVertexCount; j++, i++)
	{
		vertex2* v = &((vertex2*)mVertexData)[i];
		*v = mesh->mVerticies[j];
		v->tv = 1.0f - v->tv;
	}

	for (unsigned int i = mLastDrawIdx, j = 0; j < mesh->mPolyCount*3; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + mesh->mIndexes[j];
	}

	mTrianglesCount += 2;
	mLastDrawVertex += mesh->mVertexCount;
	mLastDrawIdx += mesh->mPolyCount*3;

	return true;
}

void grRenderSystem::setLinesWidth( float width )
{
	glLineWidth(width);
}

bool grRenderSystem::drawLines( vertex2* verticies, int count )
{
	if (!mReady)
		return false;
	
//check difference
	if (mCurrentPrimitiveType == GL_TRIANGLES)
	{
		drawPrimitives();

		mLastDrawTexture = NULL;
		mCurrentPrimitiveType = GL_LINES;
		glDisable(GL_TEXTURE_2D);
	}

//copy data
	memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], verticies, sizeof(vertex2)*count*2);

	for (unsigned int i = mLastDrawIdx, j = 0; j < (unsigned int)count*2; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + j;
	}

	mTrianglesCount += 2;
	mLastDrawVertex += count*2;
	mLastDrawIdx += count*2;

	return true;
}

void grRenderSystem::updateCameraTransforms()
{
	drawPrimitives();

	glMatrixMode(GL_MODELVIEW);
	float modelMatrix[16] = { 1, 0,  0, 0,
		                      0, 1,  0, 0,
							  0, 0,  1, 0,
							  0, 0, -1, 1 };

	if (mCurrentCamera)
	{
		float cs = cosf(mCurrentCamera->mRotation), sn = sinf(mCurrentCamera->mRotation);
		vec2f scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y), offs = mCurrentCamera->mPosition;
		
		modelMatrix[0] = cs*scale.x;  modelMatrix[1] = sn*scale.x; 
		modelMatrix[4] = -sn*scale.y; modelMatrix[5] = cs*scale.y; 
		float ofx = offs.x*scale.x, ofy = offs.y*scale.y;
		//(cs*x - sn*y), (T)(sn*x + cs*y)
		
		float cs2 = cosf(-mCurrentCamera->mRotation), sn2 = sinf(-mCurrentCamera->mRotation);
		modelMatrix[12] = cs2*ofx - sn2*ofy;     modelMatrix[13] = sn2*ofx + cs2*ofy;
	}

	glLoadMatrixf(modelMatrix);
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

void grRenderSystem::frameResized()
{
	mApplication->getOption(cApplicationOption::CLIENT_RECT, &mResolution);
}

bool grRenderSystem::bindRenderTarget( grRenderTarget* renderTarget )
{
	if (!renderTarget || !renderTarget->isReady())
		return false;

	drawPrimitives();

	glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);
	setupMatrix(renderTarget->getTexture()->getSize());

	mCurrentRenderTarget = renderTarget;

	return true;
}

bool grRenderSystem::unbindRenderTarget()
{
	if (!mCurrentRenderTarget)
		return false;

	drawPrimitives();

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	setupMatrix(mResolution.castTo<float>());

	mCurrentRenderTarget = NULL;

	return true;
}

grRenderTarget* grRenderSystem::getCurrentRenderTarget() const
{
	return mCurrentRenderTarget;
}

void grRenderSystem::clear( const color4& color /*= color4(0, 0, 0, 255)*/ )
{
	glClearColor(color.rf(), color.gf(), color.bf(), color.af());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
}

void grRenderSystem::setupMatrix( const vec2f& size )
{			
	float projMat[16];
	orthoProjMatrix(projMat, 0.0f, size.x, size.y, 0.0f, 0.0f, 10.0f);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (int)size.x, (int)size.y);
	glLoadMatrixf(projMat);
}

CLOSE_O2_NAMESPACE