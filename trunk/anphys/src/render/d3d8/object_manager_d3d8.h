#ifndef OBJECT_MANAGER_D3D8_H
#define OBJECT_MANAGER_D3D8_H

#include "../object_manager_base_interface.h"

struct grObjectManagerBase:public grObjectManagerBaseInterface
{
	/*from grObjectManagerBaseInterface
	{
		typedef std::vector<grRenderObject*> ObjectsList;

		grRender* mRender;
		ObjectsList mObjects;

		grObjectManagerBaseInterface(grRender* render): mRender(render) {}
		~grObjectManagerBaseInterface();

		grRenderObject* createObject(grRenderObject* newObject);
		bool removeObject(grRenderObject* object);

		bool removeAllObjects();
	};
	*/
	grObjectManagerBase(grRender* render):grObjectManagerBaseInterface(render) {}
};

#endif //OBJECT_MANAGER_D3D8_H