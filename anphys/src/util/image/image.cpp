#include "image.h"

#include "png_image_format.h"

cImage::cImage():mFormat(FMT_NONE), mData(NULL), mSize(0)
{
	mFilename[0] = '\0';
}

cImage::cImage( Format format, const vec2& size )
{
	create(format, size);
}

cImage::~cImage()
{
	safe_release_arr(mData);
}

void cImage::create( Format format, const vec2& size )
{
	if (mData) safe_release_arr(mData);

	short bpp[] = { 0, 4 };

	mFilename[0] = '\0';
	mFormat = format;
	mSize = size;

	mData = new unsigned char[(int)size.x*(int)size.y*bpp[format]];
}

bool cImage::load( const char* filename, ImageType type )
{
	if (type == IT_PNG) return loadPngImage(filename, this);

	return false;
}

bool cImage::save( const char* filename, ImageType type )
{
	if (type == IT_PNG) return savePngImage(filename, this);

	return false;
}

void cImage::clear( const color4& color )
{
	short bpp[] = { 0, 4 };

	memset(mData, color.dwordARGB(), bpp[mFormat]*(int)mSize.x*(int)mSize.y);
}
