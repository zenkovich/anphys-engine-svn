#ifndef LEVEL_GAMEPLAY_STATE_H
#define LEVEL_GAMEPLAY_STATE_H

#include "game_state.h"

OPEN_O2_NAMESPACE

class IGameObject;

class LeveGameplayState: public IGameState
{
public:
	typedef array<IGameObject*> GameObjectsArr;

private:
	GameObjectsArr mGameObjects;

public:
	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif // LEVEL_GAMEPLAY_STATE_H
