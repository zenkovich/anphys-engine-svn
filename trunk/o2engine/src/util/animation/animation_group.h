#ifndef ANIMATION_GROUP_H
#define ANIMATION_GROUP_H

#include "public.h"
#include "animation.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Animations group. Animates some animation with same absolute time and intervals. */
class cAnimationGroup: public IAnimation
{
public:
	typedef vector< IAnimation* > AnimationsVec;

protected:
	AnimationsVec mAnimations; /**< Animations. */

public:
	/** ctor. */
	cAnimationGroup();

	/** dtor. */
	~cAnimationGroup();

	/** Adding animation in group. */
	void addAnimation(IAnimation* animation);

	/** Removing animation. */
	void removeAnimation(IAnimation* animation);

	/** Removing all animations. */
	void removeAllAnimations();

	/** Returns vector of animations. */
	const AnimationsVec& getAnimations() const;
	
	/** Updating animation. */
	void update(float dt);

	/** Playing animation. */
	void play(float begin, float end);

	/** Playing animation. */
	void play(bool restart = false, bool stopAtEnd = true);
	
	/** Stop animation. */
	void stop();
	
	/** Setting absolute time. */
	void setTime(float time);
};

CLOSE_O2_NAMESPACE

#endif // ANIMATION_GROUP_H
