#ifndef RENDER_TARGET_OGL_H
#define RENDER_TARGET_OGL_H

#include "ogl.h"

#include "../render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** Opengl render target. */
class grRenderTarget:public grRenderTargetBaseInterface
{
	friend class grRenderSystem;

	GLuint mFrameBuffer; /**< Opengl framebuffer handle. */
	bool   mReady;       /**< True, if we can use that rander target. */

public:
	/** ctor. */
	grRenderTarget(grRenderSystem* renderSystem, grTexture* texture);
	
	/** ctor. */
	grRenderTarget(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		           grTexFormat::type texFormat = grTexFormat::DEFAULT );
	
	/** dtor. */
	~grRenderTarget();
	
	/** Returns true, if render target ready to use. */
	bool isReady() const;

protected:
	void initializeBuffer();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_OGL_H