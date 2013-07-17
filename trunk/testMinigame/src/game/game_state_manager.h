#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <vector>

class IGameState;

class GameStateManager
{
	typedef std::vector<IGameState*> GameStatesVec;

	GameStatesVec mGameStates;

	IGameState*   mLoadingGameState;
	IGameState*   mCurrentGameState;

public:
	GameStateManager();
	~GameStateManager();

	void        addState(IGameState* gameState);
	IGameState* getGameState(const char* stateName) const;

	void        setGameState(IGameState* gameState);

	void        update(float dt);
	void        draw();

private:
	void reloadGameState();
};

#endif //GAME_STATE_MANAGER_H