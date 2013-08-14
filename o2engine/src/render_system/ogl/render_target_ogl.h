#ifndef RENDER_TARGET_OGL_H
#define RENDER_TARGET_OGL_H

#include "ogl.h"

#include "../render_target_base_interface.h"

OPEN_O2_NAMESPACE

class grRenderTarget:public grRenderTargetBaseInterface
{
	friend class grRenderSystem;

	GLuint mFrameBuffer;

	bool   mReady;

public:
	grRenderTarget(grRenderSystem* renderSystem, grTexture* texture);
	grRenderTarget(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		           grTexFormat::type texFormat = grTexFormat::DEFAULT );

	~grRenderTarget();

	bool isReady() const;

protected:
	void initializeBuffer();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_OGL_H