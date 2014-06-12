#ifndef PNG_IMAGE_FORMAT_H
#define PNG_IMAGE_FORMAT_H

#include "public.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

class cImage;
class cLogStream;

bool loadPngImage(const string& fileName, cImage* image, bool errors = true, shared<cLogStream> plog = NULL);
bool savePngImage(const string& fileName, const cImage* image, shared<cLogStream> plog = NULL);

CLOSE_O2_NAMESPACE

#endif //PNG_IMAGE_FORMAT_H