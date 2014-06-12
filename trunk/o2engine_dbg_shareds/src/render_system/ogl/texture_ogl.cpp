#include "public.h"
#ifdef RENDER_OGL

#include "texture_ogl.h"

#include "util/image/image.h"
#include "../render_system.h"

OPEN_O2_NAMESPACE
	
grTextureDef::grTextureDef():
	grTextureDefBaseInterface(), mHandle(0)
{
}

grTextureDef::~grTextureDef()
{	
	glDeleteTextures(1, &mHandle);
}

void grTextureDef::create( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
						   grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	mFormat = format;
	mUsage = usage;
	mSize = size;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (format == grTexFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (format == grTexFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, (GLsizei)size.x, (GLsizei)size.y, 0, texFormat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void grTextureDef::createFromImage( shared<cImage> image )
{
	cImage::Format imageFormat = image->getFormat();
	if (imageFormat == cImage::FMT_NONE)
		mFormat = grTexFormat::DEFAULT;
	else if (imageFormat == cImage::FMT_R8G8B8A8)
		mFormat = grTexFormat::R8G8B8A8;

	mUsage = grTexUsage::DEFAULT;
	mSize = image->getSize().castTo<float>();
	mFileName = image->getFilename();

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	GLint texFormat = GL_RGB;
	if (mFormat == grTexFormat::R8G8B8A8)
		texFormat = GL_RGBA;
	else if (mFormat == grTexFormat::R8G8B8)
		texFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, image->getSize().x, image->getSize().y, 0, texFormat, GL_UNSIGNED_BYTE, 
		         image->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void grTextureDef::createFromFile( const string& fileName )
{
	shared<cImage> image = mnew cImage;
	if (image->load(fileName, cImage::IT_AUTO, renderSystem()->mLog))
		createFromImage(image);
}

void grTextureDef::createAsRenderTarget( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	create(size, format, grTexUsage::RENDER_TARGET);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL