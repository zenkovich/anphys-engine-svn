#include "ark_pad.h"

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "arkanoid_gamefield.h"


ArkanoidPad::ArkanoidPad( ArkanoidGamefield* gamefield ):mOwnerGamefield(gamefield), mWidth(50), mHeight(30), mMinWidth(30),
	mLeftSidePartSpr(NULL), mCenterPartSpr(NULL), mRightSidePartSpr(NULL)
{
	mWidth = mTargetWidth = 50;
	mMinWidth = 30;
	mLastWidthIncSpeed = 0;
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
	for (ArkPadBehaviorsVec::iterator it = mBehaiors.begin(); it != mBehaiors.end();)
	{
		(*it)->update(dt);

		if ((*it)->isComplete())
		{
			delete *it;
			it = mBehaiors.erase(it);
		}
		else ++it;
	}

	mLeftSidePartSpr->setPosition(mPosition + vec2f(-mWidth*0.5f - mLeftSidePartSpr->getSize().x, -mHeight*0.5f)).
		              setSize(vec2f(mLeftSidePartSpr->getSize().x, mHeight));

	mCenterPartSpr->setPosition(mPosition + vec2f(-mWidth*0.5f, -mHeight*0.5f)).
		            setSize(vec2f(mWidth, mHeight));

	mRightSidePartSpr->setPosition(mPosition + vec2f(mWidth*0.5f, -mHeight*0.5f)).
		               setSize(vec2f(mRightSidePartSpr->getSize().x, mHeight));
}

void ArkanoidPad::draw()
{

}

void ArkanoidPad::addBehavior( ArkPadBehavior* behavior )
{

}

void ArkanoidPad::removeBehavior( ArkPadBehavior* behavior )
{

}

void ArkanoidPad::removeAllBehaviors()
{

}

void ArkanoidPad::updateWidth( float dt )
{

}
