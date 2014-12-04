#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "public.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class IGameObject: public IDynamicDrawable
{
protected:
	vec2f mPosition;
	float mRotation;

public:
	DEFINE_TYPE(IGameObject);

	IGameObject(const vec2f& position = vec2f(), float rotation = 0);
	virtual ~IGameObject();

	virtual void update(float dt) = 0;
	virtual void draw() = 0;
};

CLOSE_O2_NAMESPACE

#endif // GAME_OBJECT_H
