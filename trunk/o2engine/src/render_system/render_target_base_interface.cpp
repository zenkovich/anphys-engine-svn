#include "render_target_base_interface.h"

#include "render_system.h"
#include "texture.h"
#include "util/log/log_stream.h"
#include "render_target.h"

OPEN_O2_NAMESPACE

grRenderTargetBaseInterface::grRenderTargetBaseInterface( grRenderSystem* renderSystem, grTexture* texture ):
	mRenderSystem(renderSystem)
{
	if (mRenderSystem == NULL)
		return;

	if (texture->getUsage() != grTexUsage::RENDER_TARGET)
	{
		mRenderSystem->mLog->out("ERROR: render target can't use not render target texture!");
	}
	else
	{
		mRenderTexture = texture;
		mRenderTexture->incRefCount();
	}
}

grRenderTargetBaseInterface::grRenderTargetBaseInterface( grRenderSystem* renderSystem, const vec2f& size /*= vec2f(0, 0)*/, 
	                                                      grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	mRenderSystem(renderSystem)
{
	if (mRenderSystem == NULL)
		return;

	vec2f texSize = size;
	if (texSize.x < 1)
	{
		texSize = mRenderSystem->getResolution().castTo<float>();
	}

	mRenderTexture = mRenderSystem->addTexture(new grTexture(mRenderSystem, texSize, texFormat, grTexUsage::RENDER_TARGET));
	mRenderTexture->incRefCount();
}

grRenderTargetBaseInterface::~grRenderTargetBaseInterface()
{
	if (mRenderTexture)
		mRenderSystem->removeTexture(mRenderTexture);
}

void grRenderTargetBaseInterface::bind()
{
	if (mRenderTexture && mRenderSystem)
		mRenderSystem->bindRenderTarget(static_cast<grRenderTarget*>(this));
}

void grRenderTargetBaseInterface::unbind()
{
	if (mRenderTexture && mRenderSystem)
		mRenderSystem->unbindRenderTarget();
}

grTexture* grRenderTargetBaseInterface::getTexture() const
{
	return mRenderTexture;
}

CLOSE_O2_NAMESPACE