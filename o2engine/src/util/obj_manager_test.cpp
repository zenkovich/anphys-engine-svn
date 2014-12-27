#include "obj_manager_test.h"

#include "assert.h"

OPEN_O2_NAMESPACE

CREATE_SINGLETON(ObjectsManager);


Object::~Object()
{
	foreach(HandlesArr, mHandles, handleIt)
		(*handleIt)->mValid = false;
}

void Object::regHandle(IObjectHandle* handle)
{
	mHandles.add(handle);
}

void Object::unregHandle(IObjectHandle* handle)
{
	mHandles.remove(handle);
	if (mHandles.count() == 0)
		ObjectsManager::instance().removeObject(mHandleIdx);
}

void Object::forceDestroy()
{
	foreach(HandlesArr, mHandles, handleIt)
		(*handleIt)->mValid = false;
	
	ObjectsManager::instance().removeObject(mHandleIdx);
}


ObjectsManager::ObjectsManager():
	mObjects(1024), mFreeHandles(1024), mReleasedObjects(512)
{
	createFreeHandles(512);
}

ObjectsManager::~ObjectsManager()
{
	processObjectsReleasing();

	int unremovedObjectsCount = 0;
	foreach(ObjectsArr, mObjects, objIt)
	{
		if (*objIt == NULL)
			continue;

		unremovedObjectsCount++;
		safe_release(*objIt);
	}

	hlog("Found %i unremoved objects", unremovedObjectsCount);
}

void ObjectsManager::createFreeHandles(int count)
{
	int lastObjCount = mObjects.count();
	int lastHandlCount = mFreeHandles.count();
	
	mObjects.resize(lastObjCount + count);
	mFreeHandles.resize(lastHandlCount + count);

	for (int i = 0; i < count; i++) 
	{
		int hdx = lastObjCount + i;
		mObjects[hdx] = NULL;
		mFreeHandles[lastHandlCount + i] = hdx;
	}
}

int ObjectsManager::addObject(Object* object)
{
	if (mFreeHandles.count() == 0)
		createFreeHandles(512);

	int handleIdx = mFreeHandles.popBack();
	mObjects[handleIdx] = object;
	object->mHandleIdx = handleIdx;

	return handleIdx;
}

void ObjectsManager::removeObject(int handleIdx)
{
	mReleasedObjects.add(mObjects[handleIdx]);
	mObjects[handleIdx] = NULL;
	mFreeHandles.add(handleIdx);
}

Object* ObjectsManager::getObjectByHandleIdx(int handleIdx)
{
	return mObjects[handleIdx];
}

void ObjectsManager::processObjectsReleasing()
{
	foreach(ObjectsArr, mReleasedObjects, objIt)
		safe_release(*objIt);

	mReleasedObjects.clear();
}

CLOSE_O2_NAMESPACE