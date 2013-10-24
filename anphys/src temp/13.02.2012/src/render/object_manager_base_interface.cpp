#include "object_manager_base_interface.h"
#include "render.h"

#include "render_object.h"
#include "object_manager.h"

#include <algorithm>
#include "../util/utils.h"

grObjectManagerBaseInterface::~grObjectManagerBaseInterface()
{
	removeAllObjects();
}

grRenderObject* grObjectManagerBaseInterface::createObject(grRenderObject* newObject)
{
	mObjects.push_back(newObject);
	newObject->setObjectManager(static_cast<grRenderObjectsManager*>(this));
	return newObject;
}

bool grObjectManagerBaseInterface::removeObject(grRenderObject* object)
{
	ObjectsList::iterator it = std::find(mObjects.begin(), mObjects.end(), object);
	if (it == mObjects.end()) return false;
	safe_release(*it);
	mObjects.erase(it);
	return true;
}

bool grObjectManagerBaseInterface::removeAllObjects()
{
	if (mObjects.size() == 0) return false;
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		safe_release(*it);

	mObjects.clear();

	return true;
}

void grObjectManagerBaseInterface::update(float dt)
{	
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		(*it)->update(dt);
}

void grObjectManagerBaseInterface::render()
{
	for (ObjectsList::iterator it = mObjects.begin(); it != mObjects.end(); it++)
		(*it)->render();
}