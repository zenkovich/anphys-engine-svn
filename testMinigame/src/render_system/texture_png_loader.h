#ifndef TEXTURE_PNG_LOADER_H
#define TEXTURE_PNG_LOADER_H

#include <Windows.h>
#include <gl/gl.h>
#include "math/vector2.h"

bool loadGLTextureFromPNG(const char* filename, GLuint* textureHandle, vec2f* size);

#endif //TEXTURE_PNG_LOADER_H