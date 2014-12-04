#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "public.h"
#include "util/type_indexation.h"
#include "util/objects.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class IGameObject: public IDynamicDrawable
{
protected:
	vec2f mPosition;
	float mRotation;

public:
	DEFINE_TYPE(IGameObject);

	PROPERTY(IGameObject, vec2f) position;
	PROPERTY(IGameObject, float) rotation;


	IGameObject(const vec2f& position = vec2f(), float rotation = 0);
	virtual ~IGameObject();

	virtual void update(float dt) = 0;
	virtual void draw() = 0;

	void setPosition(const vec2f& position);
	vec2f getPosition() const;

	void setRotation(float rotation);
	float getRotation() const;

	vec2f transformLocalToWorld(const vec2f& pointLocal);

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // GAME_OBJECT_H
