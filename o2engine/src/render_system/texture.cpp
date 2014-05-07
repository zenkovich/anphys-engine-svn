#include "texture.h"

#include "render_system.h"
#include "util/image/image.h"

OPEN_O2_NAMESPACE

grTexture::grTexture( shared(grTextureDef) object ):
	cReferenceObj(object)
{
	if (object) {
		mSize = object->mSize;
		mFormat = object->mFormat;
		mUsage = object->mUsage;
		mFileName = object->mFileName;
	}
	else {
		mSize = vec2f(1, 1);
		mFormat = grTexFormat::DEFAULT;
		mUsage = grTexUsage::DEFAULT;
	}
}

grTexture::operator grTextureDef*()
{
	return mObject;
}

grTexture grTexture::create( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
	                         grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	return grTexture(renderSystem()->createTexture(size, format, usage));
}

grTexture grTexture::createFromImage( shared(cImage) image )
{
	return grTexture(renderSystem()->createTextureFromImage(image));
}

grTexture grTexture::createFromFile( const string& fileName )
{
	return grTexture(renderSystem()->getTextureFromFile(fileName));
}

grTexture grTexture::createAsRenderTarget( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	return grTexture(renderSystem()->createRenderTargetTexture(size, format));
}

CLOSE_O2_NAMESPACE