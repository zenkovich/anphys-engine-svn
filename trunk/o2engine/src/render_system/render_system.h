#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

/* !!! More information about Render System interface see in render_system_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
	#include "ogl/render_system_ogl.h"
#elif defined(RENDER_D3D9C)
	#include "d3d9c\render_system_d3d9c.h"
#endif

//#define renderSystem() (grRenderSystem*)grRenderSystem::instancePtr()
OPEN_O2_NAMESPACE

inline grRenderSystem* renderSystem() { return (grRenderSystem*)((grRenderSystemBaseInterface*)grRenderSystem::instancePtr()); }
inline grFontManager* fontsManager() { return renderSystem()->getFontManager(); }

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_H