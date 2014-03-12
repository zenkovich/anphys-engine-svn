#include "public.h"
#ifdef RENDER_OGL

#include "render_target_ogl.h"

#include "../render_system.h"
#include "../texture.h"
#include "ogl_debug.h"
#include "ogl_ext_win.h"
#include "other/device_info.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

grRenderTarget::grRenderTarget( grTextureDef* texture ):
	grRenderTargetBaseInterface(texture), mFrameBuffer(0)
{
	if (mRenderTexture)
		initializeBuffer();
}

grRenderTarget::grRenderTarget( const vec2f& size /*= vec2f()*/, 
	                            grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	grRenderTargetBaseInterface(size, texFormat), mFrameBuffer(0)
{
	if (!mRenderTexture)
		initializeBuffer();	
}

grRenderTarget::~grRenderTarget()
{
	glDeleteBuffers(1, &mFrameBuffer);
}

void grRenderTarget::initializeBuffer()
{
	glGenFramebuffersEXT(1, &mFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER, mFrameBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture->mHandle, 0);
 
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		GLenum glError = glGetError();

		renderSystem()->mLog->out("ERROR: Failed to create GL framebuffer object! GL Error %x %s", glError,
			                      getGLErrorDesc(glError));

		mReady = false;

		return;
	}

	mReady = true;

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL