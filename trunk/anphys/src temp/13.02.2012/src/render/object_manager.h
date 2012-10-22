#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "../engine/engine_options.h"
#ifdef RENDER_D3D8
#include "d3d8/object_manager_d3d8.h"
#endif //RENDER_D3D8

struct grRenderObjectsManager:public grObjectManagerBase
{
	grRenderObjectsManager(grRender* render = NULL):grObjectManagerBase(render) {}
};

#endif //OBJECT_MANAGER_H
