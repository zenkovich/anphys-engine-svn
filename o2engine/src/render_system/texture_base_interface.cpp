#include "texture_base_interface.h"

#include "render_system.h"

OPEN_O2_NAMESPACE


grTextureInterface::grTextureInterface():
	mFormat(grTexFormat::DEFAULT), mUsage(grTexUsage::DEFAULT)
{
}

grTextureInterface::~grTextureInterface()
{
}

const std::string& grTextureInterface::getFileName() const
{
	return mFileName;
}

grTexFormat::type grTextureInterface::getFormat() const
{
	return mFormat;
}

grTexUsage::type grTextureInterface::getUsage() const
{
	return mUsage;
}

vec2f grTextureInterface::getSize() const
{
	return mSize;
}

void grTextureDefBaseInterface::onZeroRefCount()
{
	renderSystem()->removeTextureDef((grTextureDef*)this);
}

grTextureDefBaseInterface::grTextureDefBaseInterface():
	grTextureInterface()
{
}

grTextureDefBaseInterface::~grTextureDefBaseInterface()
{
}

CLOSE_O2_NAMESPACE