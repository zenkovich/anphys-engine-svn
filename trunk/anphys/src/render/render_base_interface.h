#ifndef RENDER_BASE_INTERFACE_H
#define RENDER_BASE_INTERFACE_H

#include "../util/math/mmath.h"
#include <string>
#include "../util/log/log_system.h"
#include "../util/log/log_stream_in_file.h"

struct grCamerasManager;
struct cLogStreamInFile;
struct grTextureManager;
struct grLightManager;
struct grMaterialManager;
struct grSurfaceMaterialManager;
struct grSceneManager;
struct grRender2D;

struct grRenderBaseInterface
{
	grRender2D*               mRender2D;
	grCamerasManager*         mCameras;
	cLogStreamInFile*         mRenderLog;
	grTextureManager*         mTextures;
	grLightManager*           mLights;
	grMaterialManager*        mMaterials;
	grSurfaceMaterialManager* mSurfaceMaterials;
	grSceneManager*           mSceneManager;

	grRenderBaseInterface();
	virtual ~grRenderBaseInterface();

	virtual void update(float dt);
	virtual void preRender();
	virtual void render();
	virtual void postRender();

	virtual void swapFullscreen() { *mRenderLog << formatStr("function swapFullscreen() in %s not released\n", getRenderName()); }

	virtual const char* getRenderName() { return "render base"; }
};

#endif //RENDER_BASE_INTERFACE_H