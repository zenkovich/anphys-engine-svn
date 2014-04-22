#include "render_system_base_interface.h"

#include <algorithm>

#include "util/log/file_log_stream.h"
#include "util/log.h"
#include "app/application.h"
#include "texture.h"
#include "font_manager.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(grRenderSystemBaseInterface);

grRenderSystemBaseInterface::grRenderSystemBaseInterface():
	mCurrentCamera(NULL)
{
	mLog = mnew cFileLogStream("Render", gLog->getLevel(), "render_log.txt");
	gLog->bindStream(mLog);
	mFontManager = mnew grFontManager();

	initializeProperties();
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	safe_release(mFontManager);
	gLog->unbindStream(mLog);
}

void grRenderSystemBaseInterface::bindCamera( grCamera* const& camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();
}

grCamera* grRenderSystemBaseInterface::currentCamera() const
{
	return mCurrentCamera;
}

grFontManager* grRenderSystemBaseInterface::getFontManager() const
{
	return mFontManager;
}

grTexture grRenderSystemBaseInterface::getTextureFromFile( const string& fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (fileName == (*it)->getFileName())
		{
			return grTexture(*it);
		}
	}

	grTextureDef* newTexture = mnew grTextureDef();
	newTexture->createFromFile(fileName);
	addTextureDef(newTexture);

	mLog->hout("Created texture '%s'", fileName.c_str());

	return grTexture(newTexture);
}

grTexture grRenderSystemBaseInterface::createTexture( const vec2f& size, 
	                                                  grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
													  grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	grTextureDef* res = mnew grTextureDef();
	res->create(size, format, usage);
	addTextureDef(res);
	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createTextureFromImage( cImage* image )
{
	grTextureDef* res = mnew grTextureDef();
	res->createFromImage(image);
	addTextureDef(res);
	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createRenderTargetTexture( const vec2f& size, 
	                                                              grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	grTextureDef* res = mnew grTextureDef();
	res->createAsRenderTarget(size, format);
	addTextureDef(res);
	return grTexture(res);
}

vec2i grRenderSystemBaseInterface::getResolution() const
{
	return mResolution;
}

void grRenderSystemBaseInterface::drawLine( const vec2f& a, const vec2f& b, const color4 color /*= color4(255)*/ )
{
	unsigned long dcolor = color.dword();
	vertex2 v[] = { vertex2(a.x, a.y, dcolor, 0, 0), vertex2(b.x, b.y, dcolor, 0, 0) };
	drawLines(v, 1);
}

void grRenderSystemBaseInterface::drawRectFrame( const vec2f& minp, const vec2f& maxp, const color4 color /*= color4(255)*/ )
{
	unsigned long dcolor = color.dword();
	vertex2 v[] = { vertex2(minp.x, minp.y, dcolor, 0, 0), vertex2(maxp.x, minp.y, dcolor, 0, 0),
	                vertex2(maxp.x, minp.y, dcolor, 0, 0), vertex2(maxp.x, maxp.y, dcolor, 0, 0),
	                vertex2(maxp.x, maxp.y, dcolor, 0, 0), vertex2(minp.x, maxp.y, dcolor, 0, 0),
	                vertex2(minp.x, maxp.y, dcolor, 0, 0), vertex2(minp.x, minp.y, dcolor, 0, 0) };
	drawLines(v, 4);
}

void grRenderSystemBaseInterface::drawCross( const vec2f& pos, float size /*= 5*/, const color4 color /*= color4(255)*/ )
{
	unsigned long dcolor = color.dword();
	vertex2 v[] = { vertex2(pos.x - size, pos.y, dcolor, 0, 0), vertex2(pos.x + size, pos.y, dcolor, 0, 0),
	                vertex2(pos.x, pos.y - size, dcolor, 0, 0), vertex2(pos.x, pos.y + size, dcolor, 0, 0) };
	drawLines(v, 2);
}

grTextureDef* grRenderSystemBaseInterface::addTextureDef( grTextureDef* texture )
{
	mTextures.push_back(texture);
	return texture;
}

void grRenderSystemBaseInterface::removeTextureDef( grTextureDef* texture )
{
	if (!texture/* || texture->getRefCount() > 0*/)
		return;

	TexturesVec::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);
	if (fnd != mTextures.end())
		mTextures.erase(fnd);

	safe_release(texture);
}

void grRenderSystemBaseInterface::removeAllTextures()
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
		safe_release(*it);

	mTextures.clear();
}

void grRenderSystemBaseInterface::initializeProperties()
{
	camera.init(this, &grRenderSystemBaseInterface::bindCamera, &grRenderSystemBaseInterface::currentCamera);
}

CLOSE_O2_NAMESPACE