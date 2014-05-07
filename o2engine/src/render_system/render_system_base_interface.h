#ifndef RENDER_SYSTEM_BASE_INTERFACE_H
#define RENDER_SYSTEM_BASE_INTERFACE_H

#include "public.h"
#include "util/singleton.h"
#include "util/property.h"
#include "util/math/vector2.h"
#include "util/math/color.h"
#include "util/math/vertex.h"
#include "util/math/rect.h"

#include "texture_base_interface.h"
#include "font_manager.h"

OPEN_O2_NAMESPACE

class cLogStream;
class grCamera;
class grMesh;
class grRenderTarget;
class grTexture;

/** Render system base interface. Containing resolution of render frame, textures, camera and log. */
class grRenderSystemBaseInterface: public cSingleton<grRenderSystemBaseInterface>
{
	friend class grTextureDef;
	friend class grTextureDefBaseInterface;
	friend class grRenderTargetBaseInterface;	
	friend class grFontManager;
	friend class cDeviceInfo;

public:
	typedef vector< shared(grTextureDef) > TexturesVec;

protected:
	vec2i                 mResolution;    /**< Resolution of rendering frame. */
	TexturesVec           mTextures;      /**< Textures array. */
	shared(grFontManager) mFontManager;   /**< Font manager. */
	shared(grCamera)      mCurrentCamera; /**< Current camera. Null if standard camera. */
	shared(cLogStream)    mLog;           /**< Log stream for render messages. */
	 
public:
	//properties
	PROPERTY(grRenderSystemBaseInterface, shared(grCamera)) camera; /** Camera property. Uses bindCamera and currentCamera. */


	/** ctor. */
	grRenderSystemBaseInterface();

	/** dtor. */ 
	virtual ~grRenderSystemBaseInterface();

	/** Returns resolution of rendering frame. */
	vec2i getResolution() const;

	/** Binding camera. NULL - standard camera. */
	void bindCamera(const shared(grCamera)& camera);

	/** Returns current camera. */
	shared(grCamera) currentCamera() const;

	/** Returns font manager. */
	shared(grFontManager) getFontManager() const;

	/** Creating texture, if no exist, else returning created texture. */
	grTexture getTextureFromFile(const string& fileName);
	
	/** Creates texture 
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	grTexture createTexture(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT, 
				  	        grTexUsage::type usage = grTexUsage::DEFAULT);

	/** Creates texture from image. */
	grTexture createTextureFromImage(shared(cImage) image);
				       
	/** Creates texture as render target. 
	 ** note: recommending to use grRenderTarget for rendering to texture*/
	grTexture createRenderTargetTexture(const vec2f& size, grTexFormat::type format = grTexFormat::DEFAULT);

	/** Draw single line with color. */
	void drawLine(const vec2f& a, const vec2f& b, const color4 color = color4::white());

	/** Draw rect frame with color. */
	void drawRectFrame(const vec2f& minp, const vec2f& maxp, const color4 color = color4::white());

	/** Draw cross with color. */
	void drawCross(const vec2f& pos, float size = 5, const color4 color = color4::white());

	/** Beginning rendering. */
	virtual bool beginRender() = 0;

	/** Finishing rendering. */
	virtual bool endRender() = 0;

	/** Clearing current frame buffer with color. */
	virtual void clear(const color4& color = color4::black()) = 0;

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
	virtual bool drawMesh(shared(grMesh) mesh) = 0;

	/** Drawing lines. */
	virtual bool drawLines(vertex2* verticies, int count) = 0;

	/** Sets lines width. */
	virtual void setLinesWidth(float width) = 0;
	
	/** Binding render target. */
	virtual bool bindRenderTarget(shared(grRenderTarget) renderTarget) = 0;

	/** Unbinding render target. */
	virtual bool unbindRenderTarget() = 0;

	/** Returns current render target. Returns NULL if no render target. */
	virtual shared(grRenderTarget) getCurrentRenderTarget() const = 0;

	/** Returns true, if render target is can be used with current device. */
	virtual bool isRenderTargetAvailable() const = 0;

	/** Returns maximum texture size. */
	virtual vec2i getMaxTextureSize() const = 0;

protected:
	/** Initializing properties. */
	void initializeProperties();

	/** Calls for update camera transformations. */
	virtual void updateCameraTransforms() = 0;

	/** Calls when frame changed client size. */
	virtual void frameResized() = 0;

	/** Adding texture an array and return pointer. */
	shared(grTextureDef) addTextureDef(shared(grTextureDef) texture);

	/** Removes texture. */
	void removeTextureDef(shared(grTextureDef) texture);

	/** Removes all textures. */
	void removeAllTextures();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_BASE_INTERFACE_H