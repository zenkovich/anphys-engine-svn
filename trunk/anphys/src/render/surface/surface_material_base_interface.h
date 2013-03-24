#ifndef SURFASE_MATERIAL_BASE_INTERFACE_H
#define SURFASE_MATERIAL_BASE_INTERFACE_H

#include <string>
#include <vector>
#include "util/math/mmath.h"
#include "util/log/log_system.h"

struct grSurfaceMaterialManager;
struct grMaterial;
struct grTexture;
struct grShadeModel;

struct grSurfaceMaterialBaseInterface
{
	typedef std::vector<grTexture*> TexturesList;

	grSurfaceMaterialManager* mSurfaceMaterialManager;
	TexturesList              mTextures;
	grMaterial*               mMaterial;
	grShadeModel*             mShadeModel;

//functions
	grSurfaceMaterialBaseInterface(grSurfaceMaterialManager* surfaceMaterialManager):
		mSurfaceMaterialManager(surfaceMaterialManager), mMaterial(NULL), mShadeModel(NULL) {  }
	~grSurfaceMaterialBaseInterface();

	virtual void pushTexture(grTexture* texture);
	virtual void setMaterial(grMaterial* material);
	virtual void setShadeModel(grShadeModel* shadeModel);

	virtual void setup() {}

	virtual void removeAllTextures();

	virtual const char* getStructRealization() { return "base surface material"; }
};



#endif //SURFASE_MATERIAL_BASE_INTERFACE_H
