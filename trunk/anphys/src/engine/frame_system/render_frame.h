#ifndef RENDER_FRAME_H
#define RENDER_FRAME_H

#include <string>
#include "util/math/mmath.h"
#include "util/other/string_utils.h"
#include "util/memory/mem_utils.h"
#include "util/dynamics/parameter_container.h"

#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32
#define pbFrame apWindow
#include "engine/frame_system/win32_framesystem/simple_window.h"
#endif

#define frame_type_win32_window 0 

struct grRender;
struct cEngine;
struct phEngine;

struct grRenderFrame:public pbFrame
{
	unsigned int mType;
	grRender*    mRender;
	phEngine*    mPhysics;
	cEngine*     mEngine;

	grRenderFrame();
	virtual ~grRenderFrame();

	cParameterContainer<fRect> mRect;
};

#endif //RENDER_FRAME_H