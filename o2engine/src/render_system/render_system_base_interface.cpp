#include "render_system_base_interface.h"

#include "util/image/bitmap.h"
#include "util/log/file_log_stream.h"
#include "util/log.h"
#include "app/application.h"
#include "texture.h"
#include "font_manager.h"

OPEN_O2_NAMESPACE

grRenderSystemBaseInterface::grRenderSystemBaseInterface():
	mCurrentCamera(NULL)
{
	mLog = mnew FileLogStream("Render", gLog->getLevel(), "render_log.txt");
	gLog->bindStream(mLog);
	mFontManager = mnew grFontManager();

	initializeProperties();
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	safe_release(mFontManager);
	gLog->unbindStream(mLog);
}

void grRenderSystemBaseInterface::bindCamera( grCamera* camera )
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
	FOREACH(TexturesVec, mTextures, it)
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

	mLog->hout("Created texture %ix%i, format: %s, usage: %s", (int)size.x, (int)size.y, grTexFormat::getId(format),
		       grTexUsage::getId(usage));

	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createTextureFromImage( Bitmap* image )
{
	grTextureDef* res = mnew grTextureDef();
	res->createFromImage(image);
	addTextureDef(res);

	mLog->hout("Created texture %ix%i, format: %s, usage: %s from image", (int)res->getSize().x, (int)res->getSize().y, 
		       grTexFormat::getId(res->getFormat()), grTexUsage::getId(res->getUsage()));

	return grTexture(res);
}

grTexture grRenderSystemBaseInterface::createRenderTargetTexture( const vec2f& size, 
	                                                              grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	grTextureDef* res = mnew grTextureDef();
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

void grRenderSystemBaseInterface::drawCircle(const vec2f& pos, float radius /*= 5*/, const color4 color /*= color4::white()*/)
{
	const int segCount = 20;
	vertex2 v[segCount*2];
	unsigned long dcolor = color.dword();

	float angleSeg = 2.0f*PI/(float)(segCount - 1);
	for (int i = 0; i < segCount; i++)
	{
		float a = (float)i*angleSeg;
		v[i*2]     = vertex2(vec2f::rotated(a           )*radius + pos, dcolor, 0, 0);
		v[i*2 + 1] = vertex2(vec2f::rotated(a + angleSeg)*radius + pos, dcolor, 0, 0);
	}

	drawLines(v, segCount);
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
	REG_PROPERTY_SETTER_NONCONST(grRenderSystemBaseInterface, camera, bindCamera, currentCamera);
}

CLOSE_O2_NAMESPACE