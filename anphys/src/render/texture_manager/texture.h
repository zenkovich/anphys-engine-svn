#ifndef TEXTURE_H
#define TEXTURE_H

#include "util/math/mmath.h"

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/texture/texture_d3d8.h"
#endif


struct grTexture:public grTextureBase
{
	int  mRefCount;
	bool mCanCache;

//functions
	grTexture(grTextureManager* textureManager = NULL):grTextureBase(textureManager), mRefCount(1), mCanCache(true) {}
	virtual ~grTexture() {}
};

#endif //TEXTURE_H