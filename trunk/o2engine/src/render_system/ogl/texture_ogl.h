#ifndef TEXTURE_OGL_H
#define TEXTURE_OGL_H

#include "../texture_base_interface.h"
#include "ogl.h"

OPEN_O2_NAMESPACE

/** OpenGL texture definition. Contains OGL texture handle, derives from texture definition base interface. */
class grTextureDef:public grTextureDefBaseInterface
{
	friend class grRenderSystemBaseInterface;
	friend class grRenderSystem;
	friend class grRenderTarget;

	GLuint mHandle; /**< Texture handle. */


	/** ctor. */
	grTextureDef();

	/** dtor. */
	~grTextureDef();

	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	void create(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
		  	    grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	void createFromImage(cImage* image);
		       
	/** Creates texture from file. */
	void createFromFile(const string& fileName);
		       
	/** Creates texture as render target. 
	 ** note: recommending to use grRenderTarget*/
	void createAsRenderTarget(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT);
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_OGL_H