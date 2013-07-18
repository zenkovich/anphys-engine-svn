#ifndef ARKANOID_GAMEFIELD_H
#define ARKANOID_GAMEFIELD_H

#include <vector>

class RenderSystem;
class Sprite;
class InputMessageDispatcher;

class ArkanoidPad;
class ArkanoidBall;
class ArkanoidBrick;
class ArkanoidPadBonus;

typedef std::vector<ArkanoidBall*> ArkanoidBallsVec;
typedef std::vector<ArkanoidBrick*> ArkanoidBricksVec;
typedef std::vector<ArkanoidPadBonus*> ArkanoidBonusesVec;

class ArkanoidGamefield
{
	friend class ArkanoidBall;

	enum GameState { GS_WAIT_BEGINNING = 0, GS_PLAYING, GS_LOST_LIVE, GS_FAILED };

	RenderSystem*      mRenderSystem;

	ArkanoidPad*       mPad;
	ArkanoidBallsVec   mBalls;
	ArkanoidBricksVec  mBricks;
	ArkanoidBonusesVec mBonuses;

	float              mPlayingTime;

	float              mNextBricksLineTime;

	Sprite*            mBackSprite;

public:
	ArkanoidGamefield(RenderSystem* renderSystem);
	~ArkanoidGamefield();

	void draw();
	void update(float dt);

	void setupUserController(InputMessageDispatcher* messageDispatcher);
	void setupAIController();

	void beginGame();
};

#endif //ARKANOID_GAMEFIELD_H