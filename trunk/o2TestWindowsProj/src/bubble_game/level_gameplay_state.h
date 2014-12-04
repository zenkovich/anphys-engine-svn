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
	void initializeObjects();
	void addPlayerObject();
	void addTestPad();
};

CLOSE_O2_NAMESPACE

#endif // LEVEL_GAMEPLAY_STATE_H
