#ifndef RENDER_SYSTEM_BASE_INTERFACE_H
#define RENDER_SYSTEM_BASE_INTERFACE_H

#include <vector>

#include "public.h"
OPEN_O2_NAMESPACE

class apFrame;
class grTexture;
class grCamera;
class grMesh;

class grRenderSystemBaseInterface
{
public:
	typedef std::vector<grTexture*> TexturesVec;

protected:
	apFrame*    mFrame;

	TexturesVec mTextures;

	grCamera*   mCurrentCamera;

public:
	grRenderSystemBaseInterface();
	virtual ~grRenderSystemBaseInterface();

	virtual bool beginRender() { return true; }
	virtual bool endRender() { return true; }

	bool         bindCamera(grCamera* camera);

	grTexture*   addTexture(grTexture* texture);
	grTexture*   createTexture(const std::string& fileName);
	bool         removeTexture(grTexture* texture);
	bool         removeAllTextures();

	virtual bool drawMesh(grMesh* mesh) { return true; }

protected:
	virtual void updateCameraTransforms() {}
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_BASE_INTERFACE_H