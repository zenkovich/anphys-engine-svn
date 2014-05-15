#ifndef RENDER_TARGET_D3D9C_H
#define RENDER_TARGET_D3D9C_H

#include "d3d9c.h"

#include "../render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** D3D9C render target. */
class grRenderTarget:public grRenderTargetBaseInterface
{
	friend class grRenderSystem;

	/** ctor. */
	grRenderTarget(grTextureDef* texture);

public:	
	/** ctor. */
	grRenderTarget(const vec2f& size = vec2f(), grTexFormat::type texFormat = grTexFormat::DEFAULT );
	
	/** dtor. */
	~grRenderTarget();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_D3D9C_H