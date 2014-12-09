#ifndef WATER_DROP_GAME_OBJECT_H
#define WATER_DROP_GAME_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"

OPEN_O2_NAMESPACE

class grSprite;

class WaterDropGameObject: public IGameObject, public VeretPhysics::Collider
{
	grSprite* mSprite;

public:
	DEFINE_TYPE(WaterDropGameObject);

	WaterDropGameObject(const vec2f& position = vec2f());
	~WaterDropGameObject();

	void update(float dt);
	void draw();

	void setPhysicsLayer(int layer);

	SERIALIZBLE_INHERITED_METHODS(WaterDropGameObject, IGameObject);

protected:
	void onLoad();
	void onActivate();
	void onDeactivate();
};

CLOSE_O2_NAMESPACE

#endif // WATER_DROP_GAME_OBJECT_H
