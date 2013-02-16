#ifndef RENDER_3D_OBJECT_D3D8_H
#define RENDER_3D_OBJECT_D3D8_H

#include "../render_3d_object_base_interface.h"

struct grRender3DObjectBase:public grRender3DObjectBaseInterface
{
	grRender3DObjectBase():grRender3DObjectBaseInterface() {}
	grRender3DObjectBase(grRenderObjectsManager* objectsManager):grRender3DObjectBaseInterface(objectsManager) {}
};


#endif //RENDER_3D_OBJECT_D3D8_H