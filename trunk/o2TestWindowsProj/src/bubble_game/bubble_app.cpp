#include "bubble_app.h"

#include "game_state.h"
#include "level_gameplay_state.h"
#include "game_over_game_state.h"

OPEN_O2_NAMESPACE

BubbeGameApplication::BubbeGameApplication():
	mCurrentGameState(NULL), mNextGameState(NULL)
{
	mGameStates.add(mnew LeveGameplayState());
	mGameStates.add(mnew GameOverGameState());

	foreach(GameStatesArr, mGameStates, stateIt)
		(*stateIt)->mBubbeGameApp = this;
}

BubbeGameApplication::~BubbeGameApplication()
{
	release_array(GameStatesArr, mGameStates);
}

void BubbeGameApplication::onUpdate(float dt)
{
	if (mCurrentGameState)
		mCurrentGameState->update(dt);

	if (mNextGameState)
	{
		if (mCurrentGameState)
			mCurrentGameState->onDeactivate();

		mCurrentGameState = mNextGameState;
		mCurrentGameState->onActivate();

		mNextGameState = NULL;
	}
}

void BubbeGameApplication::onDraw()
{
	if (mCurrentGameState)
		mCurrentGameState->draw();
}

void BubbeGameApplication::onStarted()
{
	setResizible(false);
	setWindowCaption("Bubble");

	goGamePlayState();
}

void BubbeGameApplication::goGamePlayState()
{
	setGameState(LeveGameplayState::getStaticType());
}

void BubbeGameApplication::goGameOverState()
{
	setGameState(GameOverGameState::getStaticType());
}

void BubbeGameApplication::setGameState(UniqueType type)
{
	if (mCurrentGameState && mCurrentGameState->getType()  == type)
		return;

	mNextGameState = getGameSate(type);
}

IGameState* BubbeGameApplication::getGameSate( UniqueType type )
{
	foreach(GameStatesArr, mGameStates, stateIt)
	{
		if ((*stateIt)->getType() == type)
			return *stateIt;
	}

	return NULL;
}

CLOSE_O2_NAMESPACE
