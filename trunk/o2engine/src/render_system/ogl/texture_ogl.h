#ifndef TEXTURE_OGL_H
#define TEXTURE_OGL_H

#include "../texture_base_interface.h"
#include "ogl.h"

OPEN_O2_NAMESPACE

class grTexture:public grTextureBaseInterface
{
	friend class grRenderSystem;
	friend class grRenderTarget;

	GLuint mHandle;

public:
	grTexture();

	grTexture(grRenderSystem* renderSystem, const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
			  grTexUsage::type usage = grTexUsage::DEFAULT);

	grTexture(grRenderSystem* renderSystem, const std::string& fileName);

	~grTexture();


	void create(grRenderSystem* renderSystem, const vec2f& size, 
	            grTexFormat::type format = grTexFormat::DEFAULT, 
		  	    grTexUsage::type usage = grTexUsage::DEFAULT);

	void createFromImage(grRenderSystem* renderSystem, cImage* image);
		       
	void createFromFile(grRenderSystem* renderSystem, const std::string& fileName);
		       
	void createAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                      grTexFormat::type format = grTexFormat::DEFAULT);
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_OGL_H