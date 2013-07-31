#include "texture_base_interface.h"

#include "render_system.h"

OPEN_O2_NAMESPACE
	
grTextureBaseInterface::grTextureBaseInterface():
	mRenderSystem(NULL), mFormat(grTexFormat::DEFAULT), mUsage(grTexUsage::DEFAULT), mRefCount(0)
{
}

grTextureBaseInterface::grTextureBaseInterface(grRenderSystem* renderSystem, const vec2f& size, grTexFormat::type format, 
						                       grTexUsage::type usage)
{
	create(renderSystem, size, format, usage);
}

grTextureBaseInterface::grTextureBaseInterface( grRenderSystem* renderSystem, const std::string& fileName ):
	mRefCount(0)
{
	createFromFile(renderSystem, fileName);
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

CLOSE_O2_NAMESPACE