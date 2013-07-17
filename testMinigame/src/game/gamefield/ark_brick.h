#ifndef ARK_BRICK_H
#define ARK_BRICK_H

#include <vector>
#include <string>
#include "math/vector2.h"

class Sprite;

class ArkanoidGamefield;
class ArkanoidBall;

class ArkanoidBrick
{
	struct LiveStage
	{
		Sprite* mSprite;
		float   mHP;

		LiveStage(float hp, Sprite* sprite);
		~LiveStage();
	};
	typedef std::vector<LiveStage> LiveStagesVec;

	typedef std::vector<std::string> BonusIdVec;

	ArkanoidGamefield* mOwnerGamefield;

	vec2f              mPosition;
	vec2f              mSize;
		               
	float              mHP;
	float              mInitialHP;

	LiveStagesVec      mLiveStages;
	BonusIdVec         mBonuses;

public:
	ArkanoidBrick(ArkanoidGamefield* gamefield, const vec2f& pos, const vec2f& size);
	~ArkanoidBrick();

	void addLiveStage(Sprite* sprite, float hp);
	void addBonus(const std::string& bonusId);

	void draw();

	bool hitBall(ArkanoidBall* ball);
};

#endif //ARK_BRICK_H