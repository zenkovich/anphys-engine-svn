#include "animation_test.h"

#include "util/input/input_message.h"
#include "app/application.h"
#include "render_system/render_system.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

AnimationTest::AnimationTest():
	mCreatingFrames(false)
{
	mAnimation.setLoop(IAnimation::LT_TOGGLE);
	mTestCallBack = callback<AnimationTest>(this, &AnimationTest::test);
}

float cf = 0.5f;

void AnimationTest::draw()
{
	const color4 frameCrossColr(255, 100, 100, 255);
	const color4 valueCrossColr(100, 100, 255, 255);
	const color4 framesLineColr(100, 255, 100, 255);
	const color4 newFrameLineColr(255, 255, 255, 255);	

	int i = 0;
	vec2f lastFrameValue;
	for (cAnimation<vec2f>::FramesVec::const_iterator it = mAnimation.getFrames().cbegin(); 
		 it != mAnimation.getFrames().cend(); ++it, i++) 
	{
		if (i > 0) 
		{
			application()->getRenderSystem()->drawLine((it - 1)->mValue, it->mValue, framesLineColr);
		}

		application()->getRenderSystem()->drawCross(it->mValue, 3, frameCrossColr);
		lastFrameValue = it->mValue;
	}
		
	application()->getRenderSystem()->drawCross((*mAnimation), 3, valueCrossColr);

	if (mCreatingFrames) 
	{
		application()->getRenderSystem()->drawLine(lastFrameValue, appInput()->getCursorPos(), newFrameLineColr);
	}
}

void AnimationTest::update( float dt )
{
	if (appInput()->isKeyPressed('C'))
		mCreatingFrames = !mCreatingFrames;

	if (appInput()->isKeyPressed(VK_SPACE))
	{
		mAnimation.play(true, false);
		mTestCallBack->call();
	}

	if (appInput()->isKeyPressed('X'))
		mAnimation.play(random(0.0f, mAnimation.getDuration()), random(0.0f, mAnimation.getDuration()));

	if (appInput()->isKeyPressed('Y'))
		mAnimation.play(random(-100.0f, 100.0f), random(-100.0f, 100.0f));

	//llog("time = %.3f/%.3f", mAnimation.getTime(), mAnimation.getDuration());

	if (mCreatingFrames) 
	{
		if (appInput()->isKeyPressed(VK_LBUTTON))
		{
			float speed = 200;
			vec2f x = appInput()->getCursorPos();
			vec2f y;
			if (mAnimation.getFrames().size() > 0)
				y = mAnimation.getFrames().back().mValue;

			mAnimation.addFrame(x, -1,	(y -x).length()/speed , false, IT_EASY_IN_OUT);
		}
	}

	mAnimation.update(dt);
}

void AnimationTest::test()
{
	llog("TEST");
}

CLOSE_O2_NAMESPACE