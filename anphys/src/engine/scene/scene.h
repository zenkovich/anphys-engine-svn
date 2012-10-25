#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <vector>

struct cObject;
struct phScene;
struct grRenderSceneBaseInterface;
struct grRenderFrame;

struct cScene
{
	typedef std::vector<cObject*> ObjectsList;

	grRenderFrame*              mRenderFrameOwner;

	ObjectsList                 mObjects;
	phScene*                    mPhysicsScene;
	grRenderSceneBaseInterface* mRenderScene;

	bool                        mReady;

	
	cScene();
	cScene(grRenderFrame* renderFrameOwner);
	~cScene();

	void initialize();

	cObject* addObject(cObject* newObject);
	bool removeObject(cObject* object);
	void removeAllObjects();

	cObject* getObject(int idx);

	ObjectsList& getObjectsList();

	void draw();
	void update(float dt);
};

#endif