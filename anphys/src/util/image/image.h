#ifndef IMAGE_H
#define IMAGE_H

#include "util/math/mmath.h"
#include "util/other/type_intexation.h"
#include "util/math/color.h"

struct cImage
{
	enum Format { FMT_NONE = 0, FMT_R8G8B8A8 };
	enum ImageType { IT_NONE = 0, IT_PNG };

	Format         mFormat;
	unsigned char* mData;
	vec2           mSize;
	char           mFilename[64];

//functions
	cImage();
	cImage(Format format, const vec2& size);
	~cImage();

	void create(Format format, const vec2& size);

	bool load(const char* filename, ImageType type);
	bool save(const char* filename, ImageType type);

	void clear(const color4& color);
};

#endif //IMAGE_H