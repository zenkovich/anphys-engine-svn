#ifndef TEXTURE_BASE_INTERFACE_H
#define TEXTURE_BASE_INTERFACE_H

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class grRenderSystem;
class cImage;

struct grTexFormat
{
	enum type { DEFAULT = 0, R8G8B8A8, R8G8B8 };
};

struct grTexUsage
{
	enum type { DEFAULT = 0, RENDER_TARGET };
};

class grTextureBaseInterface
{
protected:
	grRenderSystem*   mRenderSystem;

	vec2f             mSize;

	grTexFormat::type mFormat;
	grTexUsage::type  mUsage;

	std::string       mFileName;
	int               mRefCount;

	bool              mReady;

public:
	grTextureBaseInterface(grRenderSystem* renderSystem);
	virtual ~grTextureBaseInterface();


	virtual void create(grRenderSystem* renderSystem, const vec2f& size, 
		                grTexFormat::type format = grTexFormat::DEFAULT, 
				  	    grTexUsage::type usage = grTexUsage::DEFAULT) {}

	virtual void createFromImage(grRenderSystem* renderSystem, cImage* image) {}
				       
	virtual void createFromFile(grRenderSystem* renderSystem, const std::string& fileName) {}
				       
	virtual void createAsRenderTarget(grRenderSystem* renderSystem, const vec2f& size, 
		                              grTexFormat::type format = grTexFormat::DEFAULT) {}
				       	       
	int getRefCount() const;
				       
	void incRefCount();
				       
	void decrRefCount();
				      
	const std::string& getFileName() const;

	grTexFormat::type getFormat() const;
					   
	grTexUsage::type getUsage() const;

	vec2f getSize() const;
};

CLOSE_O2_NAMESPACE

#endif //TEXTURE_BASE_INTERFACE_H