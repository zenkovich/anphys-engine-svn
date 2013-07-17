#include "ark_brick.h"

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "arkanoid_gamefield.h"
#include "ark_ball.h"


ArkanoidBrick::LiveStage::LiveStage( float hp, Sprite* sprite )
{
	mSprite = sprite;
	mHP = hp;
}

ArkanoidBrick::LiveStage::~LiveStage()
{
	delete mSprite;
}

ArkanoidBrick::ArkanoidBrick( ArkanoidGamefield* gamefield, const vec2f& pos, const vec2f& size ):
	mOwnerGamefield(gamefield), mPosition(pos), mSize(size), mHP(0), mInitialHP(0)
{
}

ArkanoidBrick::~ArkanoidBrick()
{

}

void ArkanoidBrick::addLiveStage( Sprite* sprite, float hp )
{
	mLiveStages.push_back(LiveStage(hp, sprite));
}

void ArkanoidBrick::addBonus( const std::string& bonusId )
{
	mBonuses.push_back(bonusId);
}

void ArkanoidBrick::draw()
{
	if (mLiveStages.size() == 0)
		return;

	int currentStage = 0;

	float hpSumm = 0;
	for (int i = 0; i < mLiveStages.size(); i++)
	{
		hpSumm += mLiveStages[i].mHP;
		if (mHP > hpSumm)
		{
			currentStage = i;
			break;
		}
	}

	// 0          1          2 
	// |----10----|----20----|-----30----|
	//           10          30           60
	//hp                 25

	int nextStage = min(currentStage + 1, mLiveStages.size() - 1);

	Sprite* lessStageSprite = mLiveStages[currentStage].mSprite;
	Sprite* hightStageSprite = mLiveStages[nextStage].mSprite;

	float lessHp = hpSumm - mLiveStages[currentStage].mHP;
	float hightHp = hpSumm;

	float coef = (mHP - lessHp)/(hightHp - lessHp);
	
	lessStageSprite->setPosition(mPosition).setColor(color4(1.0f, 1.0f, 1.0f, 1.0f - coef)).draw();
	hightStageSprite->setPosition(mPosition).setColor(color4(1.0f, 1.0f, 1.0f, coef)).draw();
}

bool ArkanoidBrick::hitBall( ArkanoidBall* ball )
{
	float ballImpulse = ball->mVelocity.len()*ball->mMass;

	mHP -= ballImpulse;
}
