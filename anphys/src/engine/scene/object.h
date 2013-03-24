#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <vector>
#include "object_component.h"
#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "util/other/type_intexation.h"

struct phRigidObject;

struct cObject
{
	typedef std::vector<cObjectComponent*> ObjectComponentsList;

	ObjectComponentsList mComponents;

//functions
	cObject();
	virtual ~cObject();

	void update(float dt);
	void draw();

	cObjectComponent* addComponent(cObjectComponent* component);
	bool removeComponent(cObjectComponent* component);
	void removeAllComponents();

	template<typename T>
	T* getComponent();

	void updateComponentsDependencies();

	phRigidObject* getPhysicsRigidBody();
	vec3& position();
	mat3x3& orientation();
};

template<typename T>
T* cObject::getComponent()
{
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		if ((*it)->getType() == T::getStaticType()) return static_cast<T*>(*it);

	return NULL;
}

#endif //ENGINE_OBJECT_H
