#ifndef MULTI_OBJECT_CONTAINER_H
#define MULTI_OBJECT_CONTAINER_H

template<typename RenderObject, typename ObjectContainer>
struct grRenderMultiObject;
struct grRenderObject;

struct grObjectConditionContainer
{
	bool mDraw;

	grObjectConditionContainer():mDraw(true) {}
	~grObjectConditionContainer() {}

	virtual void setObject(grRenderObject* obj) = 0;

	virtual void setDrawing(bool flag) { mDraw = flag; }
	virtual void update(float dt) = 0;
	virtual void render() = 0;
};


#endif