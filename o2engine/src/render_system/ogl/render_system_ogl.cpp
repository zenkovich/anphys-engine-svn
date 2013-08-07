#include "render_system_ogl.h"

OPEN_O2_NAMESPACE

grRenderSystem::grRenderSystem( cApplication* application ):
	grRenderSystemBaseInterface(application)
{
	initializeGL();
}

grRenderSystem::~grRenderSystem()
{
	removeAllTextures();

	deinitializeGL();
}

bool grRenderSystem::beginRender()
{

}

bool grRenderSystem::endRender()
{

}

bool grRenderSystem::drawMesh( grMesh* mesh )
{

}

void grRenderSystem::updateCameraTransforms()
{

}

void grRenderSystem::initializeGL()
{
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
		printf("ERROR: Can't Create A GL Device Context.\n");
		return false;						
	}

	pixelFormat = ChoosePixelFormat(mHDC, &pfd);
	if (!pixelFormat)	
	{	
		printf("ERROR: Can't Find A Suitable PixelFormat.\n");
		return false;								
	}

	if (!SetPixelFormat(mHDC, pixelFormat, &pfd))	
	{
		printf("ERROR: Can't Set The PixelFormat.\n");
		return false;
	}

	mGLContext = wglCreateContext(mHDC);
	if (!mGLContext)	
	{
		printf("ERROR: Can't Create A GL Rendering Context.\n");
		return false;
	}

	if(!wglMakeCurrent(mHDC, mGLContext))			
	{
		printf("ERROR: Can't Activate The GL Rendering Context.\n");
		return false;
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
}

void grRenderSystem::deinitializeGL()
{

}

CLOSE_O2_NAMESPACE