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

	checkCapatibles();

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

	mLog->out("GL_VENDOR: %s", glGetString(GL_VENDOR));
    mLog->out("GL_RENDERER: %s", glGetString(GL_RENDERER));
    mLog->out("GL_VERSION: %s", glGetString(GL_VERSION));
    mLog->out("GL_EXTENSIONS: %s", glGetString(GL_EXTENSIONS));

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
	
	if ((mCurrentRenderTarget && mCurrentRenderTarget->mReady) || !mCurrentRenderTarget)
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
		float cs = cosf(-mCurrentCamera->mRotation), sn = sinf(-mCurrentCamera->mRotation);
		vec2f scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y), offs = mCurrentCamera->mPosition;
		float ofx = -offs.x*scale.x, ofy = -offs.y*scale.y;
		
		modelMatrix[0] = cs*scale.x;  modelMatrix[1] = sn*scale.x; 
		modelMatrix[4] = -sn*scale.y; modelMatrix[5] = cs*scale.y; 
		modelMatrix[12] = cs*ofx - sn*ofy;     modelMatrix[13] = sn*ofx + cs*ofy;
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
	if (!renderTarget)
		return false;

	drawPrimitives();

	if (renderTarget->isReady())
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);
		setupMatrix(renderTarget->getTexture()->getSize());
	}

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
	if ((mCurrentRenderTarget && mCurrentRenderTarget->isReady()) || !mCurrentRenderTarget)
	{
		glClearColor(color.rf(), color.gf(), color.bf(), color.af());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	}
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

bool grRenderSystem::isRenderTargetAvailable() const
{
	return mRenderTargetsAvailable;
}

vec2i grRenderSystem::getMaxTextureSize() const
{
	return mMaxTextureSize;
}

void grRenderSystem::checkCapatibles()
{	
//check render targets available
	char* extensions[] = { "GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit", 
	                       "GL_EXT_packed_depth_stencil" };

	mRenderTargetsAvailable = true;
	for (int i = 0; i < 4; i++)
	{
		if (!isExtensionSupported(extensions[i]))
			mRenderTargetsAvailable = false;
	}

//get max texture size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize.x);
	mMaxTextureSize.y = mMaxTextureSize.x;
}

bool grRenderSystem::isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;

	GLubyte *where, *terminator;
	/* Extension names should not have spaces. */

	where = (GLubyte *) strchr(extension, ' ');

	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */

	start = extensions;
	for (;;)
	{
		where = (GLubyte *) strstr((const char *) start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;

		start = terminator;
	}

	return false;
}

CLOSE_O2_NAMESPACE