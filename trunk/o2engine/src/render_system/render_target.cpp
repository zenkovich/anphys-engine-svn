#include "render_target.h"

#include "render_system.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grRenderTarget::grRenderTarget( grRenderSystem* renderSystem, grTexture* texture ):
	mRenderSystem(renderSystem)
{
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

grRenderTarget::grRenderTarget( grRenderSystem* renderSystem, const vec2f& size /*= vec2f(0, 0)*/, 
	                            grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	mRenderSystem(renderSystem)
{
	vec2f texSize = size;
	if (texSize.x < 1)
	{
		texSize = mRenderSystem->getResolution().castTo<float>();
	}

	mRenderTexture = mRenderSystem->addTexture(new grTexture(mRenderSystem, texSize, texFormat, grTexUsage::RENDER_TARGET));
	mRenderTexture->incRefCount();
}

grRenderTarget::~grRenderTarget()
{
	if (mRenderTexture)
		mRenderSystem->removeTexture(mRenderTexture);
}

void grRenderTarget::bind()
{

}

void grRenderTarget::unbind()
{

}

void grRenderTarget::clear( const color4& colr /*= color4(0, 0, 0, 0)*/ )
{

}

grTexture* grRenderTarget::getTexture() const
{

}

CLOSE_O2_NAMESPACE