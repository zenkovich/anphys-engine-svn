#include "render_target_base_interface.h"

#include "render_system.h"
#include "texture.h"
#include "util/log/log_stream.h"
#include "render_target.h"

OPEN_O2_NAMESPACE

grRenderTargetBaseInterface::grRenderTargetBaseInterface( grTextureDef* texture ):
	mReady(false)
{
	
	if (!renderSystem()->isRenderTargetAvailable())
	{
		renderSystem()->mLog->error("Render targets on current Renderer is not available!");
		return;
	}

	if (texture->getUsage() != grTexUsage::RENDER_TARGET)
	{
		renderSystem()->mLog->error("render target can't use not render target texture!");
	}
	else
	{
		mRenderTexture = texture;
	}
}

grRenderTargetBaseInterface::grRenderTargetBaseInterface( const vec2f& size /*= vec2f(0, 0)*/, 
	                                                      grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	mReady(false)
{	
	if (!renderSystem()->isRenderTargetAvailable())
	{
		renderSystem()->mLog->error("Render targets on current Renderer is not available!");
		mRenderTexture = NULL;
		return;
	}

	vec2f texSize = size;
	if (texSize.x < 1)
	{
		texSize = renderSystem()->getResolution().castTo<float>();
	}

	vec2i maxTextureSize = renderSystem()->getMaxTextureSize();
	if (texSize.x > maxTextureSize.x || texSize.y > maxTextureSize.y)
	{
		renderSystem()->mLog->warning("Render target size too large! size %ix%i bounding by max %ix%i",
			                          (int)texSize.x, (int)texSize.y, maxTextureSize.x, maxTextureSize.y);
		
		texSize.x = clamp<float>(texSize.x, 64.0f, (float)maxTextureSize.x);
		texSize.y = clamp<float>(texSize.y, 64.0f, (float)maxTextureSize.y);
	}

	mRenderTexture = renderSystem()->createRenderTargetTexture(texSize, texFormat);
}

grRenderTargetBaseInterface::~grRenderTargetBaseInterface()
{
	if (mRenderTexture)
		renderSystem()->removeTextureDef(mRenderTexture);
}

void grRenderTargetBaseInterface::bind()
{
	if (mRenderTexture)
		renderSystem()->bindRenderTarget(static_cast<grRenderTarget*>(this));
}

void grRenderTargetBaseInterface::unbind()
{
	if (mRenderTexture)
		renderSystem()->unbindRenderTarget();
}

grTexture grRenderTargetBaseInterface::getTexture() const
{
	return grTexture(mRenderTexture);
}

bool grRenderTargetBaseInterface::isReady() const
{
	return mReady;
}

CLOSE_O2_NAMESPACE