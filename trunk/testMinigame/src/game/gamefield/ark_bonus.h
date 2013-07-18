#ifndef ARK_BONUS_H
#define ARK_BONUS_H

#include "math/vector2.h"

class Sprite;

class ArkanoidGamefield;
class ArkanoidPad;

class ArkanoidBonus
{
	static vec2f mSize;

	ArkanoidGamefield* mOwnerGamefield;

	vec2f              mPosition;
	vec2f              mVelocity;

	Sprite*            mSprite;

public:
	ArkanoidBonus(ArkanoidGamefield* gamefield, Sprite* sprite):mOwnerGamefield(gamefield), mSprite(sprite) {}
	virtual ~ArkanoidBonus();

	void setPosVec(const vec2f& pos, const vec2f& vel) { mPosition = pos; mVelocity = vel; }

	void draw();
	void update(float dt);

	virtual void onPadCatch(ArkanoidPad* pad) {}
};

#endif //ARK_BONUS_H