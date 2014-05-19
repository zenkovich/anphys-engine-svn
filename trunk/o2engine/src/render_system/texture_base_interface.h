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

	static string getId(type tp)
	{
		static string ids[] = { asString(DEFAULT), asString(R8G8B8A8), asString(R8G8B8) };
		return ids[tp];
	}
};

/** Texture usage. */
struct grTexUsage
{
	enum type { DEFAULT = 0, RENDER_TARGET };

	static string getId(type tp)
	{
		static string ids[] = { asString(DEFAULT), asString(RENDER_TARGET) };
		return ids[tp];
	}
};

class grTexture;

/** Texture interface. Containing size, format, usage, file name. */
class grTextureInterface
{
protected:
	friend class grRenderSystemBaseInterface;
	friend class grTexture;

	vec2f             mSize;         /**< Size of texture. */
	grTexFormat::type mFormat;       /**< Texture format. */
	grTexUsage::type  mUsage;        /**< Texture usage. */
	string            mFileName;     /**< Texture file name. */

	
	/** ctor. */
	grTextureInterface();

	/** dtor. */
	virtual ~grTextureInterface();

public:		
	/** Returns file name. */
	const string& getFileName() const;

	/** Returns format of texture. */
	grTexFormat::type getFormat() const;
					   
	/** Returns usage of texture. */
	grTexUsage::type getUsage() const;

	/** Returns size of texture. */
	vec2f getSize() const;
};


/** Texture definition base interface, derives from texture interface and reference counter. */
class grTextureDefBaseInterface: public grTextureInterface, public IRefCounter 
{
protected:
	/** ctor. */
	grTextureDefBaseInterface();

	/** dtor. */
	~grTextureDefBaseInterface();

	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	virtual void create(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
				  	    grTexUsage::type usage = grTexUsage::DEFAULT) = 0;

	/** Creates texture from image. */
	virtual void createFromImage(shared<cImage> image) = 0;
				       
	/** Creates texture from file. */
	virtual void createFromFile(const string& fileName) = 0;
				       
	/** Creates texture as render target. */
	virtual void createAsRenderTarget(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT) = 0;		

	/** Runs when reference counter sets to zero. Inherited from IRefCounter. */
	void onZeroRefCount();
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_BASE_INTERFACE_H