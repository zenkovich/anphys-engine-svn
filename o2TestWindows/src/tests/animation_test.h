#ifndef ANIMATION_TEST_H
#define ANIMATION_TEST_H

#include "test_interface.h"
#include "util/animation/animation.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE


class AnimationTest: public ITest
{
	cAnimation<vec2f> mAnimation;
	bool              mCreatingFrames;

public:
	AnimationTest(cApplication* app, cInputMessage* inputMsg);

	void draw();
	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif // ANIMATION_TEST_H
