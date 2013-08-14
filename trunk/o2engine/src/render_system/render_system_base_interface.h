#ifndef RENDER_SYSTEM_BASE_INTERFACE_H
#define RENDER_SYSTEM_BASE_INTERFACE_H

#include <vector>

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE

class cApplication;
class cLogStream;
class grCamera;
class grMesh;
class grRenderTarget;
class grTexture;

class grRenderSystemBaseInterface
{
	friend class grTexture;
	friend class grRenderTargetBaseInterface;

public:
	typedef std::vector<grTexture*> TexturesVec;

protected:
	vec2i         mResolution;
	cApplication* mApplication;
	TexturesVec   mTextures;
	grCamera*     mCurrentCamera;
	cLogStream*   mLog;

public:
	grRenderSystemBaseInterface(cApplication* application);
	virtual ~grRenderSystemBaseInterface();

	virtual bool beginRender() { return true; }
	virtual bool endRender() { return true; }

	virtual void clear(const color4& color = color4(0, 0, 0, 255)) {}

	bool bindCamera(grCamera* camera);

	grTexture* addTexture(grTexture* texture);
	grTexture* createTexture(const std::string& fileName);
	bool removeTexture(grTexture* texture);
	bool removeAllTextures();

	virtual bool drawMesh(grMesh* mesh) { return true; }
	
	virtual bool bindRenderTarget(grRenderTarget* renderTarget) { return false; }
	virtual bool unbindRenderTarget() { return false; }
	virtual grRenderTarget* getCurrentRenderTarget() const { return NULL; }

	vec2i getResolution() const;

protected:
	virtual void updateCameraTransforms() {}

	virtual void frameResized() {}
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_BASE_INTERFACE_H