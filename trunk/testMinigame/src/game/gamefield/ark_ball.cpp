#include "ark_ball.h"

#include <algorithm>

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "arkanoid_gamefield.h"
#include "ark_pad.h"


ArkanoidBall::ArkanoidBall( ArkanoidGamefield* gamefield ):mOwnerGamefield(gamefield), mRadius(10), mBallSprite(NULL), 
	mSpriteBallRadius(10), mAlive(true)
{
}

ArkanoidBall::~ArkanoidBall()
{
	removeAllBehaviors();
	delete mBallSprite;
}

void ArkanoidBall::update( float dt )
{
	mPosition += mVelocity*dt;

	checkWallCollisions();
	checkPadCollision(dt);
	checkBricksCollisions();

	for (ArkBallBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end(); )
	{
		(*it)->update(dt);

		if ((*it)->isComplete())
		{
			delete *it;
			it = mBehaviors.erase(it);
		}
		else ++it;
	}

	if (mBallSprite)
	{
		float spriteScale = mRadius/mSpriteBallRadius;
		float posOffs = mSpriteBallRadius*(1.0f - spriteScale);
		mBallSprite->setPosition(mPosition - vec2f(posOffs, posOffs)).setScale(vec2f(spriteScale, spriteScale));
	}
}

void ArkanoidBall::draw()
{
	if (mBallSprite)
		mBallSprite->draw();
}

void ArkanoidBall::addBehavior( ArkBallBehavior* behavior )
{
	mBehaviors.push_back(behavior);
}

void ArkanoidBall::removeBehavior( ArkBallBehavior* behavior )
{
	ArkBallBehaviorsVec::iterator fnd = std::find(mBehaviors.begin(), mBehaviors.end(), behavior);
	if (fnd != mBehaviors.end())
		mBehaviors.erase(fnd);

	delete behavior;
}

void ArkanoidBall::removeAllBehaviors()
{
	for (ArkBallBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end(); ++it)
		delete *it;

	mBehaviors.clear();
}

void ArkanoidBall::checkWallCollisions()
{
	//wall bonucing
	vec2f areaSize = mOwnerGamefield->mRenderSystem->getResolution().castTo<float>();

	bool collidingWall = false;
	vec2f collisionNormal;
	//left wall
	if (mPosition.x -  mRadius < 0)
	{
		collidingWall = true;
		collisionNormal = vec2f(1, 0);

		mPosition .x += mRadius - mPosition.x;
	}
	//right wall
	else if (mPosition.x + mRadius > areaSize.x)
	{
		collidingWall = true;
		collisionNormal = vec2f(-1, 0);

		mPosition.x -= mPosition.x + mRadius - areaSize.x;
	}
	//up wall
	else if (mPosition.y - mRadius < 0)
	{
		collidingWall = true;
		collisionNormal = vec2f(0, 1);

		mPosition.y -= mRadius - mPosition.y;
	}
	//down wall
	else if (mPosition.y + mRadius > areaSize.y + mRadius*2.1f)
	{
		mAlive = false;
	}

	if (collidingWall)
	{
		float impulse = mVelocity*collisionNormal*2.0f;
		mVelocity += collisionNormal*impulse;

		for (ArkBallBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end(); ++it)
			(*it)->onWallTouch(collisionNormal);
	}
}

void ArkanoidBall::checkPadCollision(float dt)
{
	ArkanoidPad* pad = mOwnerGamefield->mPad;

	vec2f ctRectMin = pad->mPosition - vec2f(pad->mWidth*0.5f, pad->mHeight*0.5f);
	vec2f ctRectMax = pad->mPosition + vec2f(pad->mWidth*0.5f, pad->mHeight*0.5f);

	bool collidingPad = false;
	vec2f collisionNormal;

	if (mPosition.x < ctRectMin.x) //left circle half
	{
		vec2f padCirclePos = ctRectMin + vec2f(0, pad->mHeight*0.5f);

		vec2f diff = mPosition - padCirclePos;
		float dist = diff.len();
		float depth = mRadius + pad->mHeight - dist;

		if (depth > 0)
		{
			collidingPad = true;
			collisionNormal = diff/dist;

			mPosition += collisionNormal*depth;
		}
	}
	else if (mPosition.x > ctRectMax.x) //right circle half
	{		
		vec2f padCirclePos = ctRectMax - vec2f(0, pad->mHeight*0.5f);

		vec2f diff = mPosition - padCirclePos;
		float dist = diff.len();
		float depth = mRadius + pad->mHeight - dist;

		if (depth > 0)
		{
			collidingPad = true;
			collisionNormal = diff/dist;

			mPosition += collisionNormal*depth;
		}
	}
	else //center
	{
		if (mPosition.y + mRadius > ctRectMin.y)
		{
			collidingPad = true;
			collisionNormal = vec2f(0, -1);

			mPosition.y -= mPosition.y + mRadius - ctRectMin.y;
		}
	}

	if (collidingPad)
	{
		vec2f padVelocity = (pad->mPosition - pad->mLastPosition)/dt;

		const float padBonceCoef = 0.5f;
		const float padFrictionCoef = 0.4f;

		float impulse = (mVelocity*2.0f - padVelocity*padBonceCoef)*collisionNormal;

		vec2f resImpulse = collisionNormal*impulse + padVelocity*padFrictionCoef;

		mVelocity += resImpulse;

		for (ArkBallBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end(); ++it)
			(*it)->onPadTouch(pad);

		pad->onBallTouch(this);
	}
}

void ArkanoidBall::checkBricksCollisions()
{

}
