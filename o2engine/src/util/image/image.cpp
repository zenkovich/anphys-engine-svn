#include "image.h"

#include "png_image_format.h"
#include "util\log.h"

OPEN_O2_NAMESPACE

cImage::cImage():
	mFormat(FMT_NONE), mData(NULL)
{
}

cImage::cImage( Format format, const vec2i& size ):
	mFormat(format), mSize(size), mData(NULL)
{
	create(format, size);
}

cImage::cImage(const string& fileName, ImageType type /*= IT_AUTO*/, cLogStream* clog /*= NULL*/):
	mFormat(FMT_NONE), mData(NULL)
{
	load(fileName, type, clog);
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

bool cImage::load( const string& fileName, ImageType type, cLogStream* clog /*= NULL*/ )
{
	cLogStream* log = clog ? clog:gLog;

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

bool cImage::save( const string& fileName, ImageType type, cLogStream* clog /*= NULL*/ ) const
{
	cLogStream* log = clog ? clog:gLog;

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

void cImage::copyImage( cImage* img, const vec2i& position /*= vec2f()*/ )
{
	if (mFormat != img->mFormat)
		return;

	int pixelSize = 4;

	for (int x = 0; x < img->mSize.x; x++)
	{
		if (x + position.x >= mSize.x)
			break;

		for (int y = 0; y < img->mSize.y; y++)
		{
			if (y + position.y >= mSize.y)
				break;

			uint32 srcIdx = (img->mSize.y - y - 1)*img->mSize.x + x;
			uint32 dstIdx = (mSize.y - 1 - (y + position.y))*mSize.x + x + position.x;

			unsigned long colr = color4::blue().dword();
			memcpy(mData + dstIdx*pixelSize, img->mData + srcIdx*pixelSize, pixelSize);
		}
	}
}

void cImage::fill( const color4& color )
{
	unsigned long colrDw = color.dword();
	for (int x = 0; x < mSize.x*mSize.y; x++)
		memcpy(mData + x*4, &colrDw, 4);
}

CLOSE_O2_NAMESPACE