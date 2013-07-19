#ifndef IMAGE_H
#define IMAGE_H

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class cImage
{
public:
	enum Format { FMT_NONE = 0, FMT_R8G8B8A8 };
	enum ImageType { IT_NONE = 0, IT_PNG };

protected:
	Format         mFormat;
	unsigned char* mData;
	vec2i          mSize;
	std::string    mFilename;

public:
	cImage();
	cImage(Format format, const vec2i& size);
	virtual ~cImage();

	void create(Format format, const vec2i& size);

	bool load(const std::string& fileName, ImageType type);
	bool save(const std::string& fileName, ImageType type) const;

	void clear(const color4& color);
};

CLOSE_O2_NAMESPACE

#endif //IMAGE_H