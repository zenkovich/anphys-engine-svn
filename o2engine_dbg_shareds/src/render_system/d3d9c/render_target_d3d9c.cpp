#include "public.h"
#ifdef RENDER_D3D9C

#include "render_target_d3d9c.h"

#include "../render_system.h"
#include "../texture.h"
#include "other/device_info.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

grRenderTarget::grRenderTarget( grTextureDef* texture ):
	grRenderTargetBaseInterface(texture)
{
	mReady = true;
}

grRenderTarget::grRenderTarget( const vec2f& size /*= vec2f()*/, 
	                            grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	grRenderTargetBaseInterface(size, texFormat)
{
	mReady = true;
}

grRenderTarget::~grRenderTarget()
{
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C