#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "public.h"
#include "util/type_indexation.h"
#include "util/objects.h"
#include "util/property.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class IGameObject: public IDynamicDrawable, public cSerializable
{
protected:
	vec2f mPosition;
	float mRotation;
	bool  mActive;

public:
	DEFINE_TYPE(IGameObject);

	PROPERTY(IGameObject, vec2f) position;
	PROPERTY(IGameObject, float) rotation;
	PROPERTY(IGameObject, bool)  active;


	IGameObject(const vec2f& position = vec2f(), float rotation = 0);
	virtual ~IGameObject();

	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void setPhysicsLayer(int layer) {}

	void setPosition(const vec2f& position);
	vec2f getPosition() const;

	void setRotation(float rotation);
	float getRotation() const;

	void setActive(bool active);
	bool isActive() const;

	vec2f transformLocalToWorld(const vec2f& pointLocal);

	SERIALIZBLE_METHODS(IGameObject);

protected:
	virtual void onActivate() {}
	virtual void onDeactivate() {}

	virtual void onDeserialized();

	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // GAME_OBJECT_H
