#ifndef LEVEL_GAMEPLAY_STATE_H
#define LEVEL_GAMEPLAY_STATE_H

#include "game_state.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class IGameObject;
class VeretPhysics;
class grCamera;
class PlayerBubble;

class LeveGameplayState: public IGameState
{
public:
	enum PhysicsLayers { PL_PLAYER, PL_PAD, PL_MAX_LAYER_ID };

	typedef array<IGameObject*> GameObjectsArr;

private:
	GameObjectsArr mGameObjects;
	VeretPhysics*  mVerletPhysics;
	PlayerBubble*  mPlayer;
	grCamera*      mCamera;

public:
	DEFINE_TYPE(LeveGameplayState);

	LeveGameplayState();
	~LeveGameplayState();

	void onActivate();
	void onDeactivate();

	void update(float dt);
	void draw();

protected:
	void initPhysicsLayers();
	void initializeObjects();
	void addPadGameObject(const vec2f& position, float width = 6.0f, float rotation = 0);
	void addWaterDrop(const vec2f& position);
};

CLOSE_O2_NAMESPACE

#endif // LEVEL_GAMEPLAY_STATE_H
