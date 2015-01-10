#ifndef OBJ_MANAGER_TEST_H
#define OBJ_MANAGER_TEST_H

#include "public.h"
#include "singleton.h"

OPEN_O2_NAMESPACE

class IObjectHandle
{
	friend class Object;

protected:
	bool mValid;
};

class Object
{
	friend class ObjectsManager;

protected:
	typedef array<IObjectHandle*> HandlesArr;

	int        mHandleIdx;
	HandlesArr mHandles;

	virtual ~Object();

	void regHandle(IObjectHandle* handle);
	void unregHandle(IObjectHandle* handle);
	void forceDestroy();
};

template<typename _type>
class ObjectHandle: public IObjectHandle
{
protected:
	_type* mObject;

	ObjectHandle(_type* object);
	ObjectHandle(const ObjectHandle& objHandle);
	virtual ~ObjectHandle();

	ObjectHandle& operator=(const ObjectHandle& objHandle);

public:
	void forceDestroy();
	bool isValid();
};

class ObjectsManager: public Singleton<ObjectsManager>
{
	friend class Object;

	typedef array<Object*> ObjectsArr;
	typedef array<int> IntArr;

	ObjectsArr mObjects;
	IntArr     mFreeHandles;
	ObjectsArr mReleasedObjects;

public:
	ObjectsManager();
	~ObjectsManager();
	
private:
	int addObject(Object* object);
	void removeObject(int handleIdx);
	void processObjectsReleasing();
	void createFreeHandles(int count);

public:
	Object* getObjectByHandleIdx(int handleIdx);
};

CLOSE_O2_NAMESPACE

#endif // OBJ_MANAGER_TEST_H
