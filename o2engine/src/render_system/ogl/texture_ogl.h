#ifndef TEXTURE_OGL_H
#define TEXTURE_OGL_H

#include "../texture_base_interface.h"

OPEN_O2_NAMESPACE

class grTexture:public grTextureBaseInterface
{
public:
	grTexture() {}

	grTexture(grRenderSystem* renderSystem, const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
						   grTexUsage::type usage = grTexUsage::DEFAULT);

	grTexture(grRenderSystem* renderSystem, const std::string& fileName) {}

	~grTexture() {}


	void create(grRenderSystem* renderSystem, const vec2f& size, 
	            grTexFormat::type format = grTexFormat::DEFAULT, 
		  	    grTexUsage::type usage = grTexUsage::DEFAULT) {}
		       
	void createFromFile(grRenderSystem* renderSystem, const std::string& fileName) {}
		       
	void createAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                              grTexFormat::type format = grTexFormat::DEFAULT) {}
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_OGL_H