#ifndef MULTI_OBJECT_H
#define MULTI_OBJECT_H

#include <vector>
#include <algorithm>

#include "render/render_objects/render_object_base_interface.h"
#include "render/render_objects/object_manager.h"


template<typename RenderObject, typename ObjectContainer>
struct grRenderMultiObject:public grRenderObjectBaseInterface
{
	/* from grRenderObjectBaseInterface
	        {
				grRenderObject
				{
					enum object_types {render_3d_object = 0, render_3d_object_mesh = 1 };
					grRenderObjectsManager* mRenderObjectsManager;
					object_types mType;

					grRenderObject():mRenderObjectsManager(NULL) {}
					grRenderObject(grRenderObjectsManager* objectsManager):mRenderObjectsManager(objectManager) {}

					object_types getType() { return mType; }
				}

				grRenderObjectBaseInterface():grRenderObject() {}
				grRenderObjectBaseInterface(grRenderObjectsManager* objectsManager):grRenderObject(objectsManager) {}

				virtual void update(float dt) {  }
				virtual void render() {  }

				virtual std::string getStructRealization() { return "base render object"; }
			}		 
	*/
	typedef std::vector<ObjectContainer*> ObjectConditionsList;
	ObjectConditionsList mObjectConditions;

	RenderObject* mRenderObject;

	grRenderMultiObject():grRenderObjectBaseInterface() {}
	grRenderMultiObject(grRenderObjectsManager* objectsManager):grRenderObjectBaseInterface(objectsManager) {}
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

	virtual std::string getStructRealization() { return "grRenderMultiObjectBaseInterface"; }
};


#endif //MULTI_OBJECT_H