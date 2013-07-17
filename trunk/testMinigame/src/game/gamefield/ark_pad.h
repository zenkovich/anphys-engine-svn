#ifndef ARK_PAD_H
#define ARK_PAD_H

#include <vector>

#include "math/vector2.h"

class Sprite;

class ArkanoidGamefield;
class ArkanoidPad;
class ArkanoidBall;

class ArkPadBehavior
{
	ArkanoidPad* mOwnerPad;

public:
	ArkPadBehavior(ArkanoidPad* ownerPad):mOwnerPad(ownerPad) {}
	virtual ~ArkPadBehavior() {}

	virtual void update(float dt) {}
	virtual void draw() {}

	virtual bool isComplete() const { return false; }

	virtual void onBallTouch(ArkanoidBall* ball) {}
};
typedef std::vector<ArkPadBehavior*> ArkPadBehaviorsVec;

class ArkanoidPad
{
	ArkanoidGamefield* mOwnerGamefield;

	vec2f              mPosition;

	float              mWidth;
	float              mMinWidth;
	float              mTargetWidth;

	float              mHeight;
			           
	Sprite*            mLeftSidePartSpr;
	Sprite*            mCenterPartSpr;
	Sprite*            mRightSidePartSpr;

	ArkPadBehaviorsVec mBehaviors;

public:
	ArkanoidPad(ArkanoidGamefield* gamefield);
	~ArkanoidPad();

	void update(float dt);
	void draw();

	void addBehavior(ArkPadBehavior* behavior);
	void removeBehavior(ArkPadBehavior* behavior);
	void removeAllBehaviors();

private:
	void updateWidth(float dt);
};

#endif //ARK_PAD_H