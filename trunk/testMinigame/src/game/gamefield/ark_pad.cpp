#include "ark_pad.h"

#include <algorithm>

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "arkanoid_gamefield.h"


ArkanoidPad::ArkanoidPad( ArkanoidGamefield* gamefield ):mOwnerGamefield(gamefield), mWidth(50), mHeight(30), mMinWidth(30),
	mLeftSidePartSpr(NULL), mCenterPartSpr(NULL), mRightSidePartSpr(NULL)
{
	mWidth = mTargetWidth = 50;
	mMinWidth = 30;
}

ArkanoidPad::~ArkanoidPad()
{
	if (mLeftSidePartSpr)
		delete mLeftSidePartSpr;
	
	if (mCenterPartSpr)
		delete mCenterPartSpr;
	
	if (mRightSidePartSpr)
		delete mRightSidePartSpr;
}

void ArkanoidPad::update( float dt )
{
	for (ArkPadBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end();)
	{
		(*it)->update(dt);

		if ((*it)->isComplete())
		{
			delete *it;
			it = mBehaviors.erase(it);
		}
		else ++it;
	}

	updateWidth(dt);

	mLeftSidePartSpr->setPosition(mPosition + vec2f(-mWidth*0.5f - mLeftSidePartSpr->getSize().x, -mHeight*0.5f)).
		              setSize(vec2f(mLeftSidePartSpr->getSize().x, mHeight));

	mCenterPartSpr->setPosition(mPosition + vec2f(-mWidth*0.5f, -mHeight*0.5f)).
		            setSize(vec2f(mWidth, mHeight));

	mRightSidePartSpr->setPosition(mPosition + vec2f(mWidth*0.5f, -mHeight*0.5f)).
		               setSize(vec2f(mRightSidePartSpr->getSize().x, mHeight));
}

void ArkanoidPad::draw()
{
	mLeftSidePartSpr->draw();
	mCenterPartSpr->draw();
	mRightSidePartSpr->draw();
}

void ArkanoidPad::addBehavior( ArkPadBehavior* behavior )
{
	mBehaviors.push_back(behavior);
}

void ArkanoidPad::removeBehavior( ArkPadBehavior* behavior )
{
	ArkPadBehaviorsVec::iterator fnd = std::find(mBehaviors.begin(), mBehaviors.end(), behavior);
	if (fnd != mBehaviors.end())
		mBehaviors.erase(fnd);

	delete behavior;
}

void ArkanoidPad::removeAllBehaviors()
{
	for (ArkPadBehaviorsVec::iterator it = mBehaviors.begin(); it != mBehaviors.end(); ++it)
		delete *it;

	mBehaviors.clear();
}

void ArkanoidPad::updateWidth( float dt )
{
	if (mTargetWidth < mMinWidth)
		mTargetWidth = mMinWidth;

	float diff = mTargetWidth - mWidth;

	const float smoothDiff = 25.0f;
	const float changeSpeed = 150.0f;
	const float minDelta = 10.0;

	float speedCoef = min(diff, smoothDiff)/smoothDiff;

	float delta = sign(diff)*max(speedCoef*changeSpeed*dt, minDelta*dt);

	mWidth += delta;
}
