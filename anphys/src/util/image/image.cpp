#include "image.h"


cImage::cImage():mFormat(FMT_NONE), mData(NULL), mSize(0)
{
	mFilename[0] = '\0';
}

cImage::cImage( Format format, const vec2& size )
{

}

cImage::~cImage()
{

}

void cImage::create( Format format, const vec2& size )
{

}

bool cImage::load( const char* filename )
{

}

bool cImage::save( const char* filename, ImageType type )
{

}

void cImage::clear( const color4& color )
{

}
