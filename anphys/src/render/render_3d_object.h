#ifndef RENDER_3D_OBJECT_H
#define RENDER_3D_OBJECT_H

#include "../util/math/mmath.h"

#include "../engine/engine_options.h"
#ifdef RENDER_D3D8
#include "d3d8/render_3d_object_d3d8.h"
#endif

struct grRender3DObject:public grRender3DObjectBase
{

};

#endif //RENDER_3D_OBJECT_H