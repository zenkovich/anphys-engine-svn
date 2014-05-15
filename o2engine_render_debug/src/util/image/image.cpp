#include "image.h"

#include "png_image_format.h"
#include "util\log.h"

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

	mData = mnew unsigned char[ size.x*size.y*bpp[format] ];
}

bool cImage::load( const string& fileName, ImageType type, shared(cLogStream) clog /*= NULL*/ )
{
	shared(cLogStream) log = clog ? clog:gLog;

	mFilename = fileName;

	if (type == IT_PNG)
		return loadPngImage(fileName, this, true, log);
	else
	{
		if (loadPngImage(fileName, this, false, log)) 
			return true;

		log->error("Can't load image '%s': unknown format", fileName.c_str());
	}

	mFilename = "";

	return false;
}

bool cImage::save( const string& fileName, ImageType type, shared(cLogStream) clog /*= NULL*/ ) const
{
	shared(cLogStream) log = clog ? clog:gLog;

	if (type == IT_PNG || type == IT_AUTO)
	{
		return savePngImage(fileName, this, log);
	}

	log->error("Can't save image to '%s': unknown format specified", fileName.c_str());

	return false;
}

void cImage::clear( const color4& color )
{
	short bpp[] = { 0, 4 };

	memset(mData, color.ARGB(), bpp[mFormat]*mSize.x*mSize.y);
}

unsigned char* cImage::getData()
{
	return mData;
}

vec2i cImage::getSize() const
{
	return mSize;
}

cImage::Format cImage::getFormat() const
{
	return mFormat;
}

const unsigned char* cImage::getDataConst() const
{
	return mData;
}

const string& cImage::getFilename() const
{
	return mFilename;
}

CLOSE_O2_NAMESPACE