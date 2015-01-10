#ifndef PNG_IMAGE_FORMAT_H
#define PNG_IMAGE_FORMAT_H

#include "public.h"

OPEN_O2_NAMESPACE

class Bitmap;
class LogStream;

bool loadPngImage(const string& fileName, Bitmap* image, bool errors = true, LogStream* plog = NULL);
bool savePngImage(const string& fileName, const Bitmap* image, LogStream* plog = NULL);

CLOSE_O2_NAMESPACE

#endif //PNG_IMAGE_FORMAT_H