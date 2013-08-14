#ifndef RENDER_TARGET_BASE_INTERFACE_H
#define RENDER_TARGET_BASE_INTERFACE_H

#include "public.h"
#include "texture_base_interface.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE
	
class grRenderSystem;
class grTexture;

class grRenderTargetBaseInterface
{
protected:
	grRenderSystem* mRenderSystem;
	grTexture*      mRenderTexture;

public:
	grRenderTargetBaseInterface(grRenderSystem* renderSystem, grTexture* texture);
	grRenderTargetBaseInterface(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		                        grTexFormat::type texFormat = grTexFormat::DEFAULT );

	virtual ~grRenderTargetBaseInterface();

	void bind();
	void unbind();

	virtual bool isReady() const { return false; }

	grTexture* getTexture() const;
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_BASE_INTERFACE_H