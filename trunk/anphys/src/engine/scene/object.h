#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <vector>
#include "object_component.h"

struct cObject
{
	typedef std::vector<cObjectComponent*> ObjectComponentsList;

	ObjectComponentsList mComponents;

	cObject();
	~cObject();

	void update(float dt);
	void draw();

	cObjectComponent* addComponent(cObjectComponent* component);
	bool removeComponent(cObjectComponent* component);
	void removeAllComponents();

	cObjectComponent* getComponentByType(ObjectComponentType::types componentType);

	void updateComponentsDependencies();
};

#endif //ENGINE_OBJECT_H
