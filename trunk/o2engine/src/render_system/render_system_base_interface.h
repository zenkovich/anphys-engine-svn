#ifndef RENDER_SYSTEM_BASE_INTERFACE_H
#define RENDER_SYSTEM_BASE_INTERFACE_H

#include <vector>

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/color.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

class cApplication;
class cLogStream;
class grCamera;
class grMesh;
class grRenderTarget;
class grTexture;

/** Render system base interface. Containing resolution of render frame, textures, camera and log. */
class grRenderSystemBaseInterface
{
	friend class grTexture;
	friend class grRenderTargetBaseInterface;

public:
	typedef std::vector<grTexture*> TexturesVec;

protected:
	vec2i         mResolution;    /**< Resolution of rendering frame. */
	cApplication* mApplication;   /**< Application ptr. */
	TexturesVec   mTextures;      /**< Textures array. */
	grCamera*     mCurrentCamera; /**< Current camera. Null if standart camera. */
	cLogStream*   mLog;           /**< Log stream for render messages. */
	 
public:
	/** ctor. */
	grRenderSystemBaseInterface(cApplication* application);

	/** dtor. */ 
	virtual ~grRenderSystemBaseInterface();

	/** Beginning rendering. */
	virtual bool beginRender() { return true; }

	/** Finishing rendering. */
	virtual bool endRender() { return true; }

	/** Clearing current frame buffer with color. */
	virtual void clear(const color4& color = color4(0, 0, 0, 255)) {}

	/** Binding camera. NULL - standart camera. */
	bool bindCamera(grCamera* camera);

	/** Adding texture into textures array. */
	grTexture* addTexture(grTexture* texture);

	/** Creating texture, if no exist, else returning created texture. */
	grTexture* createTexture(const std::string& fileName);

	/** Trying to remove texture. Works by reference count. */
	bool removeTexture(grTexture* texture);

	/** Forcible removing all textures. */
	bool removeAllTextures();

	/** Drawing mesh. */
	virtual bool drawMesh(grMesh* mesh) { return true; }

	/** Drawing lines. */
	virtual bool drawLines(vertex2* verticies, int count) { return true; }

	/** Sets lines width. */
	virtual void setLinesWidth(float width) {  }
	
	/** Binding render target. */
	virtual bool bindRenderTarget(grRenderTarget* renderTarget) { return false; }

	/** Unbinding render target. */
	virtual bool unbindRenderTarget() { return false; }

	/** Returns current render target. Returns NULL if no render target. */
	virtual grRenderTarget* getCurrentRenderTarget() const { return NULL; }

	/** Returns resolution of rendering frame. */
	vec2i getResolution() const;

protected:
	/** Calls for update camera transformations. */
	virtual void updateCameraTransforms() {}

	/** Calls when frame changed client size. */
	virtual void frameResized() {}
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_BASE_INTERFACE_H