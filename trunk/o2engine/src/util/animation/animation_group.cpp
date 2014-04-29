#include "animation_group.h"

#include "util/math/math.h"

OPEN_O2_NAMESPACE

cAnimationGroup::cAnimationGroup()
{
}

cAnimationGroup::~cAnimationGroup()
{
	removeAllAnimations();
}

void cAnimationGroup::addAnimation( ptr(IAnimation) animation )
{
	mAnimations.push_back(animation);
	mDuration = max(mDuration, animation->getDuration());
}

void cAnimationGroup::removeAnimation( ptr(IAnimation) animation )
{
	AnimationsVec::iterator fnd = std::find(mAnimations.begin(), mAnimations.end(), animation);
	if (fnd != mAnimations.end())
		mAnimations.erase(fnd);

	safe_release(animation);

	mDuration = 0;
	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
	{
		if (mDuration < (*it)->getDuration())
			mDuration = (*it)->getDuration();
	}
}

void cAnimationGroup::removeAllAnimations()
{
	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		safe_release(*it);

	mAnimations.clear();
	mDuration = 0;
}

const cAnimationGroup::AnimationsVec& cAnimationGroup::getAnimations() const
{
	return mAnimations;
}

void cAnimationGroup::play( float begin, float end )
{
	IAnimation::play(begin, end);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->play(begin, end);
}

void cAnimationGroup::setTime( float time )
{
	IAnimation::setTime(time);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->setTime(time);
}

void cAnimationGroup::update( float dt )
{
	IAnimation::update(dt);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->update(dt);
}

void cAnimationGroup::stop()
{
	IAnimation::stop();

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->stop();
}

CLOSE_O2_NAMESPACE