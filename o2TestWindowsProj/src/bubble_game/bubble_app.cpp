#include "bubble_app.h"

#include "game_state.h"
#include "level_gameplay_state.h"

OPEN_O2_NAMESPACE

BubbeGameApplication::BubbeGameApplication():
	mCurrentGameState(NULL)
{
	mGameStates.add(mnew LeveGameplayState());
}

BubbeGameApplication::~BubbeGameApplication()
{
	release_array(GameStatesArr, mGameStates);
}

void BubbeGameApplication::onUpdate(float dt)
{
	if (mCurrentGameState)
		mCurrentGameState->update(dt);
}

void BubbeGameApplication::onDraw()
{
	if (mCurrentGameState)
		mCurrentGameState->draw();
}

void BubbeGameApplication::onStarted()
{
	goGamePlayState();
}

void BubbeGameApplication::goGamePlayState()
{
	setGameState(LeveGameplayState::getStaticType());
}

void BubbeGameApplication::goGameOverState()
{

}

void BubbeGameApplication::goMainMenuState()
{

}

void BubbeGameApplication::setGameState(UniqueType type)
{
	if (mCurrentGameState && mCurrentGameState->getType()  == type)
		return;

	if (mCurrentGameState)
		mCurrentGameState->onDeactivate();

	foreach(GameStatesArr, mGameStates, stateIt)
	{
		if ((*stateIt)->getType() == type)
		{
			mCurrentGameState = *stateIt;
		}
	}

	mCurrentGameState->onActivate();
}

CLOSE_O2_NAMESPACE
