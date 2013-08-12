#ifndef RENDER_SYSTEM_OGL_H
#define RENDER_SYSTEM_OGL_H

#include "render_system/render_system_base_interface.h"

#include "ogl.h"

#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

class grRenderSystem:public grRenderSystemBaseInterface
{	
	friend class cApplication;
	friend class grSprite;

	enum { nVertexBufferSize = 6000, nIndexBufferSize = 6000*3 };

//gl context
	HGLRC mGLContext;
	HDC   mHDC;
	vec2i mResolution;

//vertex & index buffers
	unsigned char*          mVertexData;          /**< Pointer to vertex data buffer. */
	unsigned short*         mVertexIndexData;
	
//batching parametres
	grTexture*              mLastDrawTexture;     /**< Stored texture ptr from last DIP. */
	unsigned int            mLastDrawVertex;      /**< Last vertex idx for next DIP. */
	unsigned int            mLastDrawIdx;         /**< Last vertex index for nex DIP. */
	unsigned int            mTrianglesCount;      /**< Triatgles count for next DIP. */
	unsigned int            mFrameTrianglesCount; /**< Total triangles at current frame. */
	unsigned int            mDIPCount;            /**< DrawIndexedPrimitives calls count. */

	bool                    mReady;               /**< True, if render system initialized. */

public:
	grRenderSystem(cApplication* application);
	~grRenderSystem();

	bool beginRender();
	bool endRender();

	bool drawMesh(grMesh* mesh);

protected:
	void updateCameraTransforms();

	void initializeGL();
	void deinitializeGL();

	void drawPrimitives();

	void frameResized();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_OGL_H