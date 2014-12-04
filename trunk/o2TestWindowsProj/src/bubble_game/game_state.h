#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "public.h"

OPEN_O2_NAMESPACE

class IGameState
{
public:
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
};

CLOSE_O2_NAMESPACE

#endif // GAME_STATE_H
