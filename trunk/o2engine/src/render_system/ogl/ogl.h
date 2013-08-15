#ifndef OGL_H
#define OGL_H

// OpenGL inlude file.
// Including files by platform

#include "engine_settings.h"

#ifdef PLATFORM_WIN

#include <windows.h>	
#include <GL/gl.h>
#include <GL/glu.h>
#include "gl/glext.h"
#include "gl/wglext.h"

#endif

#endif //OGL_H