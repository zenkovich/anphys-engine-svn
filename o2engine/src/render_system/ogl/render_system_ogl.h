#ifndef RENDER_SYSTEM_OGL_H
#define RENDER_SYSTEM_OGL_H

#include "render_system/render_system_base_interface.h"

#include "ogl.h"

#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

/** Render system, using OpenGL. */
class grRenderSystem:public grRenderSystemBaseInterface
{	
	friend class cApplication;
	friend class grSprite;
	friend class grRenderTarget;
	friend class cDeviceInfo;
	
	static const unsigned int mVertexBufferSize = 6000;  /** Maximum size of vertex buffer. */
	static const unsigned int mIndexBufferSize = 6000*3; /** Maximum size of index buffer. */

//gl context
	HGLRC           mGLContext;            /**< OpenGL context. */
	HDC             mHDC;                  /**< Win frame device context. */
										   
//vertex & index buffers				   
	unsigned char*  mVertexData;           /**< Vertex data buffer. */
	unsigned short* mVertexIndexData;      /**< Index data buffer. */
	GLenum          mCurrentPrimitiveType; /**< TYpe of drawing primitives for next DIP. */
										   
//batching parametres					   
	grTexture*      mLastDrawTexture;      /**< Stored texture ptr from last DIP. */
	unsigned int    mLastDrawVertex;       /**< Last vertex idx for next DIP. */
	unsigned int    mLastDrawIdx;          /**< Last vertex index for nex DIP. */
	unsigned int    mTrianglesCount;       /**< Triatgles count for next DIP. */
	unsigned int    mFrameTrianglesCount;  /**< Total triangles at current frame. */
	unsigned int    mDIPCount;             /**< DrawIndexedPrimitives calls count. */
										   
	grRenderTarget* mCurrentRenderTarget;  /**< Current render target. NULL if rendering in back buffer. */
										   
	bool            mReady;                /**< True, if render system initialized. */

public:
	/* ctor. */
	grRenderSystem(cApplication* application);

	~grRenderSystem();

	bool beginRender();
	bool endRender();

	void clear(const color4& color = color4(0, 0, 0, 255));

	bool drawMesh(grMesh* mesh);
	
	bool drawLines(vertex2* verticies, int count);

	void setLinesWidth(float width);

	bool bindRenderTarget(grRenderTarget* renderTarget);
	bool unbindRenderTarget();
	grRenderTarget* getCurrentRenderTarget() const;	

	/** Returns true, if render target is can be used with current device. */
	bool isRenderTargetAvailable();

	/** Returns maximum texture size. */
	vec2i getMaxTextureSize();

protected:
	void updateCameraTransforms();

	void initializeGL();
	void deinitializeGL();

	void drawPrimitives();

	void frameResized();

	void setupMatrix(const vec2f& size);

	static bool isExtensionSupported(const char *extension);
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_OGL_H