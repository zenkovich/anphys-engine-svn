#ifndef PNG_IMAGE_FORMAT_H
#define PNG_IMAGE_FORMAT_H

#include "public.h"

OPEN_O2_NAMESPACE

class cImage;

bool loadPngImage(const std::string& fileName, cImage* image);
bool savePngImage(const std::string& fileName, cImage* image);

CLOSE_O2_NAMESPACE

#endif //PNG_IMAGE_FORMAT_H