#ifndef MULTI_OBJECT_H
#define MULTI_OBJECT_H

#include <vector>
#include <algorithm>

#include "render/render_objects/render_object.h"
#include "render/render_objects/object_manager.h"


template<typename RenderObject, typename ObjectContainer>
struct grRenderMultiObject:public grRenderObject
{
	typedef std::vector<ObjectContainer*> ObjectConditionsList;

	ObjectConditionsList mObjectConditions;
	RenderObject*        mRenderObject;

//functions
	grRenderMultiObject():grRenderObject() {}
	grRenderMultiObject(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}
	~grRenderMultiObject() { clear(); safe_release(mRenderObject); }
	
	virtual void setRenderObject(RenderObject* object)
	{
		clear();
		mRenderObject = object;
	}

	virtual void setObjectManager(grRenderObjectsManager* objectsManager)
	{
		mRenderObjectsManager = objectsManager;
	}

	virtual void render()
	{
		int i = 0;
		for (ObjectConditionsList::iterator it = mObjectConditions.begin(); it != mObjectConditions.end(); it++, i++)
		{
			(*it)->render();
		}
	}

	virtual void update(float dt)
	{
		for (ObjectConditionsList::iterator it = mObjectConditions.begin(); it != mObjectConditions.end(); it++)
			(*it)->update(dt);
	}

	virtual ObjectContainer* pushObject(ObjectContainer *obj)
	{
		obj->setObject(mRenderObject);
		mObjectConditions.push_back(obj);
		return obj;
	}

	virtual bool removeObject(ObjectContainer* obj)
	{
		ObjectConditionsList::iterator it = std::find(mObjectConditions.begin(), mObjectConditions.end(), obj);

		if (it == mObjectConditions.end()) return false;

		safe_release(obj);
		mObjectConditions.erase(it);

		return true;
	}

	virtual void clear()
	{
		for (ObjectConditionsList::iterator it = mObjectConditions.begin(); it != mObjectConditions.end(); it++)
			safe_release(*it);

		mObjectConditions.clear();
	}
};


#endif //MULTI_OBJECT_H