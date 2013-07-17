#include "game_state_manager.h"

#include "game_state.h"


GameStateManager::GameStateManager()
{
	mCurrentGameState = NULL;
	mLoadingGameState = NULL;
}

GameStateManager::~GameStateManager()
{
	if (mCurrentGameState && mCurrentGameState->isLoaded())
		mCurrentGameState->unload();

	for (GameStatesVec::iterator it = mGameStates.begin(); it != mGameStates.end(); ++it)
	{
		delete *it;
	}
}

void GameStateManager::addState( IGameState* gameState )
{
	mGameStates.push_back(gameState);
}

IGameState* GameStateManager::getGameState( const char* stateName ) const
{
	for (GameStatesVec::const_iterator it = mGameStates.cbegin(); it != mGameStates.cend(); ++it)
	{
		if (strcmp((*it)->getName(), stateName) == 0)
			return *it;
	}

	return NULL;
}

void GameStateManager::setGameState( IGameState* gameState )
{
	mLoadingGameState = gameState;
}

void GameStateManager::update( float dt )
{
	if (mLoadingGameState)
		reloadGameState();

	if (mCurrentGameState)
		mCurrentGameState->update(dt);
}

void GameStateManager::draw()
{
	if (mCurrentGameState)
		mCurrentGameState->draw();
}

void GameStateManager::reloadGameState()
{
	if (mCurrentGameState)
		mCurrentGameState->unload();

	if (mLoadingGameState)
	{
		mCurrentGameState = mLoadingGameState;
		mCurrentGameState->load();
	}
}
