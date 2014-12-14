#ifndef GAME_OVER_GAME_STATE_H
#define GAME_OVER_GAME_STATE_H

#include "game_state.h"
#include "util/animation/animation_group.h"
#include "util/animation/animation.h"

OPEN_O2_NAMESPACE

class grText;
class grSprite;

class GameOverGameState: public IGameState
{
	grText*            mGameText;
	grText*            mOverText;
	grText*            mScoreText;
	grText*            mPressText;
	grSprite*          mBack;
	cAnimation<vec2f>* mGameTextAnimation;
	cAnimation<vec2f>* mOverTextAnimation;
	cAnimation<vec2f>* mScoreTextAnimation;
	cAnimation<vec2f>* mPressTextAnimation;
	cAnimationGroup    mAllAnimation;

public:
	DEFINE_TYPE(GameOverGameState);

	GameOverGameState();
	~GameOverGameState();

	void onActivate();
	void onDeactivate();

	void update(float dt);
	void draw();

	void setScore(int score);
};

CLOSE_O2_NAMESPACE

#endif // GAME_OVER_GAME_STATE_H
