#ifndef OBJECT_MANAGER_D3D8_H
#define OBJECT_MANAGER_D3D8_H

#include "render/render_objects/object_manager_base_interface.h"

struct grObjectManagerBase:public grObjectManagerBaseInterface
{
	grObjectManagerBase(grRender* render):grObjectManagerBaseInterface(render) {}
};

#endif //OBJECT_MANAGER_D3D8_H