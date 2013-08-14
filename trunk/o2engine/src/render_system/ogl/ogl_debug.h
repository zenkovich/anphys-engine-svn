#ifndef OGL_DEBUG_H
#define OGL_DEBUG_H

#include "ogl.h"
#include "public.h"

OPEN_O2_NAMESPACE

const char* getGLErrorDesc(GLenum errorId);

class cLogStream;
void glCheckError(cLogStream* log, const char* filename = NULL, unsigned int line = 0);

#ifdef RENDER_DEBUG
#	define GL_CHECK_ERROR(log) glCheckError(log, __FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR(log) 
#endif

CLOSE_O2_NAMESPACE

#endif //OGL_DEBUG_H