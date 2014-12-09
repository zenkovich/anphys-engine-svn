#ifndef WATER_DROP_GAME_OBJECT_H
#define WATER_DROP_GAME_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"

OPEN_O2_NAMESPACE

class grSprite;

class WaterDropGameObject: public IGameObject, public VeretPhysics::Collider
{
	float     mRadius;
	grSprite* mSprite;

public:
	DEFINE_TYPE(WaterDropGameObject);

	WaterDropGameObject(const vec2f& position);
	~WaterDropGameObject();

	void update(float dt);
	void draw();

	void setPhysicsLayer(int layer);

protected:
	void onDeactivate();
};

CLOSE_O2_NAMESPACE

#endif // WATER_DROP_GAME_OBJECT_H
