#ifndef PNG_IMAGE_FORMAT_H
#define PNG_IMAGE_FORMAT_H

struct cImage;

bool loadPngImage(const char* filename, cImage* image);
bool savePngImage(const char* filename, cImage* image);

#endif //PNG_IMAGE_FORMAT_H