#ifndef TEXTURE_D3D9C_H
#define TEXTURE_D3D9C_H

#include "../texture_base_interface.h"
#include "d3d9c.h"

OPEN_O2_NAMESPACE

/** Direct3D9 texture definition. Contains D3D texture pointer, derives from texture definition base interface. */
class grTextureDef:public grTextureDefBaseInterface
{
	friend class grRenderSystemBaseInterface;
	friend class grRenderSystem;
	friend class grRenderTarget;

	LPDIRECT3DTEXTURE9 mTexturePtr; /**< Direct3D9 texture pointer. */


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
	void createFromFile(const std::string& fileName);
		       
	/** Creates texture as render target. */
	void createAsRenderTarget(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT);
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_D3D9C_H