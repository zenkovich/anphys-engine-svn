#ifndef TEXTURE_H
#define TEXTURE_H

/* !!! More information about grTexture interface see in texture_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
	#include "ogl/texture_ogl.h"
#elif defined(RENDER_D3D9C)
	#include "d3d9c/texture_d3d9c.h"
#endif

OPEN_O2_NAMESPACE

class grTexture: public cReferenceObj<grTextureDef>, public grTextureInterface
{
	friend class grRenderSystem;
	friend class grRenderTargetBaseInterface;
	friend class grMesh;

	/** type convertion operator. */
	operator grTextureDef*();

public:
	/** ctor. */
	grTexture(grTextureDef* object = NULL);
	
	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	static grTexture create(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
				  	        grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	static grTexture createFromImage(cImage* image);
				       
	/** Creates texture from file. */
	static grTexture createFromFile(const std::string& fileName);
				       
	/** Creates texture as render target. */
	static grTexture createAsRenderTarget(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT);
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_H