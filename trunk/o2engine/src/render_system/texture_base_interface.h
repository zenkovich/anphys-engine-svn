#ifndef TEXTURE_BASE_INTERFACE_H
#define TEXTURE_BASE_INTERFACE_H

#include "public.h"
#include "util/ref_counter.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class grRenderSystem;
class cImage;

/** Texture format. */
struct grTexFormat
{
	enum type { DEFAULT = 0, R8G8B8A8, R8G8B8 };
};

/** Texture usage. */
struct grTexUsage
{
	enum type { DEFAULT = 0, RENDER_TARGET };
};

class grTexture;

/** Texture base interface. COntaining size, format, usage, file name, reference count. */
class grTextureBaseInterface: public IRefCounter
{
	friend class grRenderSystemBaseInterface;
	friend class grTextureRef;

protected:
	grRenderSystem*   mRenderSystem; /**< Render system ptr. */

	vec2f             mSize;         /**< Size of texture. */
	grTexFormat::type mFormat;       /**< Texture format. */
	grTexUsage::type  mUsage;        /**< Texture usage. */
	std::string       mFileName;     /**< Texture file name. */
	bool              mReady;        /**< True, if texture ready for using. */

	
	/** ctor. */
	grTextureBaseInterface();

	/** dtor. */
	virtual ~grTextureBaseInterface();

	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	virtual void createSelf(grRenderSystem* renderSystem, const vec2f& size, 
		                    grTexFormat::type format = grTexFormat::DEFAULT, 
				  	        grTexUsage::type usage = grTexUsage::DEFAULT) {}

	/** Creates texture from image. */
	virtual void createSelfFromImage(grRenderSystem* renderSystem, cImage* image) {}
				       
	/** Creates texture from file. */
	virtual void createSelfFromFile(grRenderSystem* renderSystem, const std::string& fileName) {}
				       
	/** Creates texture as render target. */
	virtual void createSelfAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                                  grTexFormat::type format = grTexFormat::DEFAULT) {}		

	/** Runs when reference counter sets to zero. Inherited from IRefCounter. */
	void onZeroRefCount();

public:						
	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	static grTextureRef create(grRenderSystem* renderSystem, const vec2f& size, 
		                       grTexFormat::type format = grTexFormat::DEFAULT, 
				  	           grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	static grTextureRef createFromImage(grRenderSystem* renderSystem, cImage* image);
				       
	/** Creates texture from file. */
	static grTextureRef createFromFile(grRenderSystem* renderSystem, const std::string& fileName);
				       
	/** Creates texture as render target. */
	static grTextureRef createAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                                     grTexFormat::type format = grTexFormat::DEFAULT);

	/** Returns file name. */
	const std::string& getFileName() const;

	/** Returns format of texture. */
	grTexFormat::type getFormat() const;
					   
	/** Returns usage of texture. */
	grTexUsage::type getUsage() const;

	/** Returns size of texture. */
	vec2f getSize() const;
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_BASE_INTERFACE_H