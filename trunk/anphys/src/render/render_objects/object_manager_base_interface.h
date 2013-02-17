#ifndef OBJECT_MANAGER_BASE_INTERFACE_H
#define OBJECT_MANAGER_BASE_INTERFACE_H

#include <string>
#include <vector>

#include "util/math/mmath.h"
#include "util/log/log_system.h"


struct grRender;
struct grRenderObject;

struct grObjectManagerBaseInterface
{
	typedef std::vector<grRenderObject*> ObjectsList;

	grRender* mRender;
	ObjectsList mObjects;

	grObjectManagerBaseInterface(grRender* render): mRender(render) {}
	~grObjectManagerBaseInterface();

	grRenderObject* createObject(grRenderObject* newObject);
	bool removeObject(grRenderObject* object);

	bool removeAllObjects();

	void update(float dt);
	void render();
};

#endif //OBJECT_MANAGER_BASE_INTERFACE_H