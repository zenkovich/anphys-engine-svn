#include "animation_group.h"

#include "util/math/math.h"

OPEN_O2_NAMESPACE

AnimationGroup::AnimationGroup()
{
}

AnimationGroup::~AnimationGroup()
{
	removeAllAnimations();
}

void AnimationGroup::addAnimation( IAnimation* animation )
{
	mAnimations.push_back(animation);
	mDuration = max(mDuration, animation->getDuration());
}

void AnimationGroup::removeAnimation( IAnimation* animation )
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

void AnimationGroup::removeAllAnimations()
{
	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		safe_release(*it);

	mAnimations.clear();
	mDuration = 0;
}

const AnimationGroup::AnimationsVec& AnimationGroup::getAnimations() const
{
	return mAnimations;
}

void AnimationGroup::play( float begin, float end )
{
	IAnimation::play(begin, end);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->play(begin, end);
}

void AnimationGroup::play( bool restart /*= false*/, bool stopAtEnd /*= true*/ )
{
	IAnimation::play(restart, stopAtEnd);
}

void AnimationGroup::setTime( float time )
{
	IAnimation::setTime(time);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->setTime(time);
}

void AnimationGroup::update( float dt )
{
	IAnimation::update(dt);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->update(dt);
}

void AnimationGroup::stop()
{
	IAnimation::stop();

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->stop();
}

CLOSE_O2_NAMESPACE