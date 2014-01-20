#ifndef RENDER_SYSTEM_BASE_INTERFACE_H
#define RENDER_SYSTEM_BASE_INTERFACE_H

#include <vector>

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/color.h"
#include "util/math/vertex.h"
#include "util/math/rect.h"

#include "texture_base_interface.h"

OPEN_O2_NAMESPACE

class cApplication;
class cLogStream;
class grCamera;
class grMesh;
class grRenderTarget;
class grTexture;
class grFontManager;

/** Render system base interface. Containing resolution of render frame, textures, camera and log. */
class grRenderSystemBaseInterface
{
	friend class grTexture;
	friend class grRenderTargetBaseInterface;	
	friend class grFontManager;
	friend class cDeviceInfo;

public:
	typedef std::vector<grTexture*> TexturesVec;

protected:
	vec2i          mResolution;    /**< Resolution of rendering frame. */
	cApplication*  mApplication;   /**< Application ptr. */
	TexturesVec    mTextures;      /**< Textures array. */
	grFontManager* mFontManager;   /**< Font manager. */
	grCamera*      mCurrentCamera; /**< Current camera. Null if standart camera. */
	cLogStream*    mLog;           /**< Log stream for render messages. */
	 
public:
	/** ctor. */
	grRenderSystemBaseInterface(cApplication* application);

	/** dtor. */ 
	virtual ~grRenderSystemBaseInterface();

	/** Returns resolution of rendering frame. */
	vec2i getResolution() const;

	/** Binding camera. NULL - standart camera. */
	bool bindCamera(grCamera* camera);

	/** Returns font manager. */
	grFontManager* getFontManager() const;

	/** Creating texture, if no exist, else returning created texture. */
	grTexture* getTextureFromFile(const std::string& fileName);
	
	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	grTexture* createTexture(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
				  	         grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	grTexture* createTextureFromImage(cImage* image);
				       
	/** Creates texture as render target. 
	 ** note: recomending to use grRenderTarget for rendering to texture*/
	grTexture* createRenderTargetTexture(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT);

	/** Trying to remove texture. Works by reference count. */
	void releaseTexture(grTexture* texture);

	/** Forcible removing all textures. */
	void removeAllTextures();

	/** Draw single line with color. */
	void drawLine(const vec2f& a, const vec2f& b, const color4 color = color4(255));

	/** Draw rect frame with color. */
	void drawRectFrame(const vec2f& minp, const vec2f& maxp, const color4 color = color4(255));

	/** Draw cross with color. */
	void drawCross(const vec2f& pos, float size = 5, const color4 color = color4(255));

	/** Beginning rendering. */
	virtual bool beginRender() = 0;

	/** Finishing rendering. */
	virtual bool endRender() = 0;

	/** Clearing current frame buffer with color. */
	virtual void clear(const color4& color = color4(0, 0, 0, 255)) = 0;

	/** Beginning render to stencil buffer. */
	virtual void beginRenderToStencilBuffer() = 0;

	/** Finishing rendering in stencil buffer. */
	virtual void endRenderToStencilBuffer() = 0;

	/** Enabling stencil test. */
	virtual void enableStencilTest() = 0;

	/** Disabling stencil test. */
	virtual void disableStencilTest() = 0;

	/** Returns true, if stencil test enabled. */
	virtual bool isStencilTestEnabled() const = 0;

	/** Clearing stencil buffer. */
	virtual void clearStencil() = 0;

	/** Sets scissor rect. */
	virtual void setupScissorRect(const fRect& rect) = 0;

	/** Returns scissor rect. */
	virtual const fRect& getScissorRect() const = 0;

	/** Enabling scissor test. */
	virtual void enableScissorTest() = 0;

	/** Disabling scissor test. */
	virtual void disableScissorTest() = 0;

	/** Returns true, if scissor test enabled. */
	virtual bool isScissorTestEnabled() const = 0;

	/** Drawing mesh. */
	virtual bool drawMesh(grMesh* mesh) = 0;

	/** Drawing lines. */
	virtual bool drawLines(vertex2* verticies, int count) = 0;

	/** Sets lines width. */
	virtual void setLinesWidth(float width) = 0;
	
	/** Binding render target. */
	virtual bool bindRenderTarget(grRenderTarget* renderTarget) = 0;

	/** Unbinding render target. */
	virtual bool unbindRenderTarget() = 0;

	/** Returns current render target. Returns NULL if no render target. */
	virtual grRenderTarget* getCurrentRenderTarget() const = 0;

	/** Returns true, if render target is can be used with current device. */
	virtual bool isRenderTargetAvailable() const = 0;

	/** Returns maximum texture size. */
	virtual vec2i getMaxTextureSize() const = 0;

protected:
	/** Calls for update camera transformations. */
	virtual void updateCameraTransforms() = 0;

	/** Calls when frame changed client size. */
	virtual void frameResized() = 0;

	/** Adding texture an array and return pointer. */
	grTexture* addTexture(grTexture* texture);
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_BASE_INTERFACE_H