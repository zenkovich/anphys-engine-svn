#ifndef BUBBLE_APP_H
#define BUBBLE_APP_H

#include "app/application.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class IGameState;

class BubbeGameApplication: public cApplication
{
public:
	typedef array<IGameState*> GameStatesArr;

private:
	GameStatesArr mGameStates;
	IGameState*   mCurrentGameState;
	IGameState*   mNextGameState;

public:	
	/** ctor. */
	BubbeGameApplication();

	/** dtor. */
	~BubbeGameApplication();

	/** Updating frame. */
	void onUpdate(float dt);

	/** Drawing frame. */
	void onDraw();

	void goGamePlayState();

	void goGameOverState();

	IGameState* getGameSate(UniqueType type);

public:
	/** Calls when application is starting. */
	void onStarted();

	void setGameState(UniqueType type);
};

CLOSE_O2_NAMESPACE

#endif // BUBBLE_APP_H
