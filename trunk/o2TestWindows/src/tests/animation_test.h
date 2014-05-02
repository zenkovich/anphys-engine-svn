#ifndef ANIMATION_TEST_H
#define ANIMATION_TEST_H

#include "test_interface.h"
#include "util/animation/animation.h"
#include "util/math/vector2.h"
#include "util/smart_ptrs.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE


class AnimationTest: public ITest
{
	cAnimation<vec2f> mAnimation;
	bool              mCreatingFrames;

	sharedPtr(ICallback) mTestCallBack;

public:
	AnimationTest();

	void draw();
	void update(float dt);

	void test();
};

CLOSE_O2_NAMESPACE

#endif // ANIMATION_TEST_H
