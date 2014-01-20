#include "texture_base_interface.h"

#include "render_system.h"

OPEN_O2_NAMESPACE


grTextureBaseInterface::grTextureBaseInterface():
	mRenderSystem(NULL), mFormat(grTexFormat::DEFAULT), mUsage(grTexUsage::DEFAULT), mRefCount(0)
{
}

grTextureBaseInterface::~grTextureBaseInterface()
{
}

int grTextureBaseInterface::getRefCount() const
{
	return mRefCount;
}

void grTextureBaseInterface::incRefCount()
{
	mRefCount++;
}

void grTextureBaseInterface::decrRefCount()
{
	mRefCount--;
}

const std::string& grTextureBaseInterface::getFileName() const
{
	return mFileName;
}

grTexFormat::type grTextureBaseInterface::getFormat() const
{
	return mFormat;
}

grTexUsage::type grTextureBaseInterface::getUsage() const
{
	return mUsage;
}

vec2f grTextureBaseInterface::getSize() const
{
	return mSize;
}

grTexture* grTextureBaseInterface::create( grRenderSystem* renderSystem, const vec2f& size, 
	                                       grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
									       grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	return renderSystem->createTexture(size, format, usage);
}

grTexture* grTextureBaseInterface::createFromImage( grRenderSystem* renderSystem, cImage* image )
{
	return renderSystem->createTextureFromImage(image);
}

grTexture* grTextureBaseInterface::createFromFile( grRenderSystem* renderSystem, const std::string& fileName )
{
	return renderSystem->getTextureFromFile(fileName);
}

grTexture* grTextureBaseInterface::createAsRenderTarget( grRenderSystem* renderSystem, const vec2f& size, 
	                                                     grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	return renderSystem->createRenderTargetTexture(size, format);
}

CLOSE_O2_NAMESPACE