#include "text_test.h"

#include "app/application.h"
#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/font_manager.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

cTextTest::cTextTest():
	mTouchCorner(-1)
{
	grRenderSystem* rs = renderSystem();
	rs->getFontManager()->loadBMFont("arial");
	mText = mnew grText(rs, rs->getFontManager()->getFont("arial"));
	mText->ctext = "Prived, i'm super\nmany strings\ntext!!\nWith digits 1234567890\nAnd Looooooooooooooooooooooooooooooooooooooooooong liiiiiiiiiiiiiiiiineeeeeeeeeeeeeeeesssssssssssssss";
	mText->rect = fRect(100, 100, 300, 300);
}

void cTextTest::update( float dt )
{	
	if (appInput()->isCursorPressed()) 
	{
		fRect textRect = mText->rect;
		vec2f corners[] = { textRect.getltCorner(), textRect.getrtCorner(), textRect.getrdCorner(), textRect.getldCorner() };

		mTouchCorner = -1;

		for (int i = 0; i < 4; i++)
		{
			if (length(corners[i], appInput()->getCursorPos()) < 5.0f)
			{
				mTouchCorner = i;
				hlog("Touched %i corner", i);
			}
		}

		if (mTouchCorner < 0 && textRect.isInside(appInput()->getCursorPos()))
			mTouchCorner = 4;
	}

	if (appInput()->isCursorDown() && mTouchCorner) 
	{
		if (mTouchCorner == 0) 
		{
			mText->position += appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 1) 
		{
			mText->areaSize += vec2f(appInput()->getCursorDelta().x, 0);
			mText->position += vec2f(0, appInput()->getCursorDelta().y);
		}		
		else if (mTouchCorner == 2) 
		{
			mText->areaSize += appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 3)
		{
			mText->position += vec2f(appInput()->getCursorDelta().x, 0);
			mText->areaSize += vec2f(0, appInput()->getCursorDelta().y);
		}
		else if (mTouchCorner == 4) 
		{
			mText->position += appInput()->getCursorDelta();
		}
	}

	if (appInput()->isCursorReleased()) 
		mTouchCorner = -1;
}

void cTextTest::draw()
{
	mText->draw();

	//debug
	fRect textRect = mText->rect;
	vec2f corners[] = { textRect.getltCorner(), textRect.getrtCorner(), textRect.getrdCorner(), textRect.getldCorner() };

	grRenderSystem* rs = renderSystem();
	rs->drawLine(corners[0], corners[1], color4::red());
	rs->drawLine(corners[0], corners[3], color4::blue());
	rs->drawLine(corners[1], corners[2], color4::white());
	rs->drawLine(corners[3], corners[2], color4::white());

	for (int i = 0; i < 4; i++) 
		rs->drawRectFrame(corners[i] - vec2f(3, 3), corners[i] + vec2f(3, 3));
}

CLOSE_O2_NAMESPACE