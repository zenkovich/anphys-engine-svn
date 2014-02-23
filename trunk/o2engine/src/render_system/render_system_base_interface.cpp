#include "render_system_base_interface.h"

#include <algorithm>

#include "util/log/file_log_stream.h"
#include "util/log.h"
#include "app/application.h"
#include "texture.h"
#include "font_manager.h"

OPEN_O2_NAMESPACE

grRenderSystemBaseInterface::grRenderSystemBaseInterface( cApplication* application ):
	mCurrentCamera(NULL), mApplication(application)
{
	mLog = mnew cFileLogStream("Render", gLog->getLevel(), "render_log.txt");
	gLog->bindStream(mLog);
	mFontManager = mnew grFontManager((grRenderSystem*)this);
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	safe_release(mFontManager);
	removeAllTextures();
	gLog->unbindStream(mLog);
}

bool grRenderSystemBaseInterface::bindCamera( grCamera* camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();

	return true;
}

grFontManager* grRenderSystemBaseInterface::getFontManager() const
{
	return mFontManager;
}

grTexture* grRenderSystemBaseInterface::getTextureFromFile( const std::string& fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (fileName == (*it)->getFileName())
		{
			(*it)->incRefCount();
			return *it;
		}
	}

	grTexture* newTexture = mnew grTexture();
	newTexture->createSelfFromFile((grRenderSystem*)this, fileName);
	newTexture->incRefCount();

	mLog->hout("Created texture '%s'", fileName.c_str());

	return addTexture(newTexture);
}

grTexture* grRenderSystemBaseInterface::createTexture( const vec2f& size, 
	                                                   grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
													   grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	grTexture* res = mnew grTexture();
	res->createSelf((grRenderSystem*)this, size, format, usage);
	res->incRefCount();
	return res;
}

grTexture* grRenderSystemBaseInterface::createTextureFromImage( cImage* image )
{
	grTexture* res = mnew grTexture();
	res->createSelfFromImage((grRenderSystem*)this, image);
	res->incRefCount();
	return res;
}

grTexture* grRenderSystemBaseInterface::createRenderTargetTexture( const vec2f& size, 
	                                                               grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	grTexture* res = mnew grTexture();
	res->createSelfAsRenderTarget((grRenderSystem*)this, size, format);
	res->incRefCount();
	return res;
}

void grRenderSystemBaseInterface::removeTexture( grTexture* texture )
{
	if (!texture || texture->getRefCount() > 0)
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

grTexture* grRenderSystemBaseInterface::addTexture( grTexture* texture )
{
	mTextures.push_back(texture);
	return texture;
}

CLOSE_O2_NAMESPACE