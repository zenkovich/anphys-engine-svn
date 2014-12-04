#ifndef BUBBLE_APP_H
#define BUBBLE_APP_H

#include "app/application.h"

OPEN_O2_NAMESPACE

class IGameState;

class BubbeGameApplication: public cApplication
{
public:
	typedef array<IGameState*> GameStatesArr;

private:
	GameStatesArr mGameStates;

public:	
	/** ctor. */
	BubbeGameApplication();

	/** dtor. */
	~BubbeGameApplication();

	/** Updating frame. */
	void onUpdate(float dt);

	/** Drawing frame. */
	void onDraw();

public:
	/** Calls when application is starting. */
	void onStarted();
};

CLOSE_O2_NAMESPACE

#endif // BUBBLE_APP_H
