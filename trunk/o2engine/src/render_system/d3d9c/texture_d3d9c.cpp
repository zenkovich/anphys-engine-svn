#include "public.h"
#ifdef RENDER_D3D9C

#include "texture_d3d9c.h"

#include "../render_system.h"
#include "util/image/image.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE
	
grTextureDef::grTextureDef():
	grTextureDefBaseInterface(), mTexturePtr(0)
{
}

grTextureDef::~grTextureDef()
{	
	if (mTexturePtr)
		mTexturePtr->Release();
}

void grTextureDef::create( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/, 
						   grTexUsage::type usage /*= grTexUsage::DEFAULT*/ )
{
	mFormat = format;
	mUsage = usage;
	mSize = size;
	
	D3DSURFACE_DESC desc;

	renderSystem()->mBackBufferSurface->GetDesc(&desc);
	renderSystem()->mBackBufferSurface->Release();

	D3DFORMAT texFormat = desc.Format;
	if (mFormat == grTexFormat::R8G8B8A8)
		texFormat = D3DFMT_A8R8G8B8;
	else if (mFormat == grTexFormat::R8G8B8)
		texFormat = D3DFMT_R8G8B8;

	DWORD dusage = 0;
	D3DPOOL dpool = D3DPOOL_MANAGED;
	if (usage == grTexUsage::RENDER_TARGET)
	{
		dusage = D3DUSAGE_RENDERTARGET;
		dpool = D3DPOOL_DEFAULT;
	}

	if (FAILED(renderSystem()->mDirect3DDevice->CreateTexture((unsigned int)mSize.x, (unsigned int)mSize.y, 
		                                                      1, dusage, texFormat, dpool, &mTexturePtr, NULL)))
	{
		renderSystem()->mLog->error("Failed to create texture!");
		return;
	}
}

void grTextureDef::createFromImage( cImage* image )
{

	cImage::Format imageFormat = image->getFormat();
	if (imageFormat == cImage::FMT_NONE)
		mFormat = grTexFormat::DEFAULT;
	else if (imageFormat == cImage::FMT_R8G8B8A8)
		mFormat = grTexFormat::R8G8B8A8;

	mUsage = grTexUsage::DEFAULT;
	mSize = image->getSize().castTo<float>();
	mFileName = image->getFilename();

	D3DFORMAT texFormat = D3DFMT_R8G8B8;
	if (mFormat == grTexFormat::R8G8B8A8)
		texFormat = D3DFMT_A8R8G8B8;
	else if (mFormat == grTexFormat::R8G8B8)
		texFormat = D3DFMT_R8G8B8;

	if (FAILED(renderSystem()->mDirect3DDevice->CreateTexture((unsigned int)mSize.x, (unsigned int)mSize.y, 
		                                                      1, 0, texFormat, D3DPOOL_MANAGED, &mTexturePtr, NULL)))
	{
		renderSystem()->mLog->error("Failed to create texture! Image:%s", image->getFilename());
		return;
	}

	D3DLOCKED_RECT lockedRect;
	if (FAILED(mTexturePtr->LockRect(0, &lockedRect, 0, D3DLOCK_DISCARD)))
	{
		renderSystem()->mLog->error("Failed to lock texture rect");
		return;
	}
	
	unsigned char* psrc = (unsigned char*)image->getData();
	for (unsigned int y = 0; y < (unsigned int)mSize.y; ++y)
	{
		unsigned char* ptr = (unsigned char*)lockedRect.pBits + lockedRect.Pitch*((unsigned int)mSize.y - y - 1);
		for (unsigned int i = 0; i < (unsigned int)mSize.x; ++i)
		{
			ptr[0] = psrc[2]; ptr[1] = psrc[1];
			ptr[2] = psrc[0]; ptr[3] = psrc[3];
			ptr+=4; psrc+=4;
		}
	}

	mTexturePtr->UnlockRect(0);
}

void grTextureDef::createFromFile( const string& fileName )
{
	cImage* image = mnew cImage;
	if (image->load(fileName, cImage::IT_AUTO, renderSystem()->mLog))
		createFromImage(image);
}

void grTextureDef::createAsRenderTarget( const vec2f& size, grTexFormat::type format /*= grTexFormat::DEFAULT*/ )
{
	create(size, format, grTexUsage::RENDER_TARGET);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C