#ifndef ARK_BALL_H
#define ARK_BALL_H

#include <vector>
#include "math/vector2.h"

class Sprite;

class ArkanoidGamefield;

class ArkanoidBall;
class ArkanoidPad;
class ArkanoidBrick;

class ArkBallBehavior
{
	ArkanoidBall* mOwnerBall;

public:
	ArkBallBehavior(ArkanoidBall* ball):mOwnerBall(ball) {}
	virtual ~ArkBallBehavior() {}

	virtual void update(float dt) {}
	virtual void draw() {}

	virtual bool isComplete() const { return false; }

	virtual void onPadTouch(ArkanoidPad* pad) {}
	virtual void onBrickTouch(ArkanoidBrick* brick) {}
};
typedef std::vector<ArkBallBehavior*> ArkBallBehaviorsVec;

class ArkanoidBall
{
	ArkanoidGamefield*  mOwnerGamefield;

	float               mRadius;
	float               mMass;
	vec2f               mPosition;
	vec2f               mVelocity;

	ArkBallBehaviorsVec mBehaviors;

	Sprite*             mBallSprite;
	float               mSpriteBallRadius;

public:
	ArkanoidBall(ArkanoidGamefield* gamefield);
	~ArkanoidBall();

	void update(float dt);
	void draw();

	void addBehavior(ArkBallBehavior* behavior);
	void removeBehavior(ArkBallBehavior* behavior);
	void removeAllBehaviors();
};

#endif //ARK_BALL_H