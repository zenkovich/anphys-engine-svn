#ifndef RENDER_TARGET_OGL_H
#define RENDER_TARGET_OGL_H

#include "ogl.h"

#include "../render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** Opengl render target. */
class grRenderTarget:public grRenderTargetBaseInterface
{
	friend class grRenderSystem;

	GLuint mFrameBuffer; /**< Opengl frame buffer handle. */

	/** ctor. */
	grRenderTarget(shared<grTextureDef> texture);

public:	
	/** ctor. */
	grRenderTarget(const vec2f& size = vec2f(), grTexFormat::type texFormat = grTexFormat::DEFAULT );
	
	/** dtor. */
	~grRenderTarget();

protected:
	void initializeBuffer();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_OGL_H