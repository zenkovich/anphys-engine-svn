#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <vector>
#include "object_component.h"
#include "../../util/math/vector.h"
#include "../../util/math/matrix.h"

struct phRigidObject;

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

	phRigidObject* getPhysicsRigidBody();
	vec3& position();
	mat3x3& orientation();
};

#endif //ENGINE_OBJECT_H
