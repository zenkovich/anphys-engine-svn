#include "image.h"

OPEN_O2_NAMESPACE

cImage::cImage():
	mFormat(FMT_NONE), mData(NULL)
{
}

cImage::cImage( Format format, const vec2i& size ):
	mFormat(format), mSize(size)
{
	create(format, size);
}

cImage::~cImage()
{
	safe_release_arr(mData);
}

void cImage::create( Format format, const vec2i& size )
{
	if (mData) 
		safe_release_arr(mData);

	short bpp[] = { 0, 4 };

	mFormat = format;
	mSize = size;

	mData = new unsigned char[ size.x*size.y*bpp[format] ];
}

bool cImage::load( const std::string& fileName, ImageType type )
{
	return false;
}

bool cImage::save( const std::string& fileName, ImageType type ) const
{
	return false;
}

void cImage::clear( const color4& color )
{
	short bpp[] = { 0, 4 };

	memset(mData, color.ARGB(), bpp[mFormat]*mSize.x*mSize.y);
}

CLOSE_O2_NAMESPACE