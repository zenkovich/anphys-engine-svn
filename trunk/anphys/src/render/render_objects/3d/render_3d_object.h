#ifndef RENDER_3D_OBJECT_H
#define RENDER_3D_OBJECT_H

#include "util/math/mmath.h"
#include "util/other/type_intexation.h"

#include "engine/engine_options.h"
#ifdef RENDER_D3D8
#include "spec_render/render_d3d8/objects/render_3d_object_d3d8.h"
#endif

struct grRender3DObject:public grRender3DObjectBase
{
	grRender3DObject():grRender3DObjectBase() {}
	grRender3DObject(grRenderObjectsManager* objectsManager):grRender3DObjectBase(objectsManager) {}
};

#endif //RENDER_3D_OBJECT_H