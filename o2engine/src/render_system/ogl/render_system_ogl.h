#ifndef RENDER_SYSTEM_OGL_H
#define RENDER_SYSTEM_OGL_H

#include "render_system/render_system_base_interface.h"

#include <windows.h>	
#include <GL/gl.h>
#include <GL/glu.h>

#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class grRenderSystem:public grRenderSystemBaseInterface
{
	HGLRC mGLContext;
	HDC   mHDC;
	vec2i mResolution;

public:
	grRenderSystem(cApplication* application);
	~grRenderSystem();

	bool beginRender();
	bool endRender();

	bool drawMesh(grMesh* mesh);

protected:
	void updateCameraTransforms();

	void initializeGL();
	void deinitializeGL();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_OGL_H