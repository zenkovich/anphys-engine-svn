#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "public.h"
#include "texture_base_interface.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE

class grRenderSystem;
class grTexture;

class grRenderTarget
{
	grRenderSystem* mRenderSystem;
	grTexture*      mRenderTexture;

public:
	grRenderTarget(grRenderSystem* renderSystem, grTexture* texture);
	grRenderTarget(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		           grTexFormat::type texFormat = grTexFormat::DEFAULT );

	~grRenderTarget();

	void bind();
	void unbind();

	void clear(const color4& colr = color4(0, 0, 0, 0));

	grTexture* getTexture() const;
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_H