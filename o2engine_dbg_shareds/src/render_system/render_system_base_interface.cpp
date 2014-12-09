#include "render_system_base_interface.h"

#include "util/image/image.h"
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

void grRenderSystemBaseInterface::bindCamera( const shared<grCamera>& camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();
}

shared<grCamera> grRenderSystemBaseInterface::currentCamera() const
{
	return mCurrentCamera;
}

shared<grFontManager> grRenderSystemBaseInterface::getFontManager() const
{
	return mFontManager;
}

grTexture grRenderSystemBaseInterface::getTextureFromFile( const string& fileName )
{
	FOREACH(TexturesVec, mTextures, it)
	{
		if (fileName == (*it)->getFileName())
		{
			return grTexture(*it);
		}
	}

	shared<grTextureDef> newTexture = mnew grTextureDef();
	newTexture->createFromFile(fileName);
	addTextureDef(newTexture);

	mLog->hout("Created texture '%s'", fileName.c_str());

	return grTexture(newTexture);
}

grTexture grRenderSystemBaseInterface::createTexture( const vec2f& size, 
	                                                  grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
													  grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	shared<grTextureDef> res = mnew grTextureDef();
	res->create(size, format, usage);
	addTextureDef(res);

	mLog->hout("Created texture %ix%i, format: %s, usage: %s", (int)size.x, (int)size.y, grTexFormat::getId(format),
		       grTexUsage::getId(usage));

	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createTextureFromImage( shared<cImage> image )
{
	shared<grTextureDef> res = mnew grTextureDef();
	res->createFromImage(image);
	addTextureDef(res);

	mLog->hout("Created texture %ix%i, format: %s, usage: %s from image", (int)res->getSize().x, (int)res->getSize().y, 
		       grTexFormat::getId(res->getFormat()), grTexUsage::getId(res->getUsage()));

	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createRenderTargetTexture( const vec2f& size, 
	                                                              grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	shared<grTextureDef> res = mnew grTextureDef();
	res->createAsRenderTarget(size, format);
	addTextureDef(res);

	mLog->hout("Created render texture %ix%i, format: %s", (int)size.x, (int)size.y, grTexFormat::getId(res->getFormat()));

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

shared<grTextureDef> grRenderSystemBaseInterface::addTextureDef( shared<grTextureDef> texture )
{
	mTextures.push_back(texture);
	return texture;
}

void grRenderSystemBaseInterface::removeTextureDef( shared<grTextureDef> texture )
{
	if (!texture/* || texture->getRefCount() > 0*/)
		return;

	
	TexturesVec::iterator fnd = FIND(mTextures, texture);
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
	REG_PROPERTY(grRenderSystemBaseInterface, camera, bindCamera, currentCamera);
}

CLOSE_O2_NAMESPACE