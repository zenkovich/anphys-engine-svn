#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "public.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class IGameState
{
public:
	DEFINE_TYPE(IGameState);

	virtual void onActivate() = 0;
	virtual void onDeactivate() = 0;

	virtual void update(float dt) = 0;
	virtual void draw() = 0;
};

CLOSE_O2_NAMESPACE

#endif // GAME_STATE_H
