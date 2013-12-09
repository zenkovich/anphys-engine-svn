#include "render_system_base_interface.h"

#include <algorithm>

#include "util/log/file_log_stream.h"
#include "util/log.h"
#include "app/application.h"
#include "texture.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grRenderSystemBaseInterface::grRenderSystemBaseInterface( cApplication* application ):
	mCurrentCamera(NULL), mApplication(application)
{
	mLog = mnew cFileLogStream("Render", gLog->getLevel(), "render_log.txt");
	gLog->bindStream(mLog);
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	removeAllTextures();
	gLog->unbindStream(mLog);
}

bool grRenderSystemBaseInterface::bindCamera( grCamera* camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();

	return true;
}

grTexture* grRenderSystemBaseInterface::addTexture( grTexture* texture )
{
	mTextures.push_back(texture);
	return texture;
}

grTexture* grRenderSystemBaseInterface::createTexture( const std::string& fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (fileName == (*it)->getFileName())
		{
			(*it)->incRefCount();
			return *it;
		}
	}

	grTexture* newTexture = mnew grTexture((grRenderSystem*)this, fileName);
	//newTexture->incRefCount();

	mLog->hout("Created texture '%s'", fileName.c_str());

	return addTexture(newTexture);
}

bool grRenderSystemBaseInterface::removeTexture( grTexture* texture )
{
	texture->decrRefCount();

	if (texture->getRefCount() == 0)
	{
		bool inVec = false;

		TexturesVec::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);
		if (fnd != mTextures.end())
		{
			mTextures.erase(fnd);
			inVec = true;
		}

		safe_release(texture);

		return inVec;
	}

	return true;
}

bool grRenderSystemBaseInterface::removeAllTextures()
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
		safe_release(*it);

	mTextures.clear();

	return true;
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

CLOSE_O2_NAMESPACE