#include "text_test.h"

#include "app/application.h"
#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/font_manager.h"
#include "util/log.h"
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

cTextTest::cTextTest():
	mTouchCorner(-1)
{
	grRenderSystem* rs = renderSystem();
	rs->getFontManager()->loadBMFont("arial");
	mText = mnew grText(rs->getFontManager()->getFont("arial"));
	mText->ctext = "Prived, i'm super\nmany strings\ntext!!\nWith digits 1234567890\nAnd Looooooooooooooooooooooooooooooooooooooooooong liiiiiiiiiiiiiiiiineeeeeeeeeeeeeeeesssssssssssssss";
	//mText->ctext = "qwertyuioplkjhgfdsazxcvbnm1234567890";
	mText->rect = fRect(100, 100, 300, 300);
	mText->gradientColorBottom = color4::red();
	mText->gradientColorTop = color4::green();
	mText->shadowColor = color4::black();
	mText->borderColor = color4::black();
}

void cTextTest::update( float dt )
{	
	mText->ctext = format("o2 test: FPS: %.1f, DT: %.3f ms", timeUtils()->getFPS(), timeUtils()->getDeltaTime()*1000.0f);

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

	if (appInput()->isCursorDown() && mTouchCorner >=0) 
	{
		if (mTouchCorner == 0) 
		{
			mText->position += appInput()->getCursorDelta();
			mText->areaSize -= appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 1) 
		{
			mText->areaSize += vec2f(appInput()->getCursorDelta().x, -appInput()->getCursorDelta().y);
			mText->position += vec2f(0, appInput()->getCursorDelta().y);
		}		
		else if (mTouchCorner == 2) 
		{
			mText->areaSize += appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 3)
		{
			mText->position += vec2f(appInput()->getCursorDelta().x, 0);
			mText->areaSize += vec2f(-appInput()->getCursorDelta().x, appInput()->getCursorDelta().y);
		}
		else if (mTouchCorner == 4) 
		{
			mText->position += appInput()->getCursorDelta();
		}
		
		fRect textRect = mText->rect;
	}

	if (appInput()->isCursorReleased()) 
		mTouchCorner = -1;

	if (appInput()->isKeyPressed('Q'))
		mText->wordWrap = !mText->wordWrap;

	if (appInput()->isKeyPressed('W'))
		mText->cursive = !mText->cursive;

	if (appInput()->isKeyPressed('E'))
		mText->bold = !mText->bold;

	if (appInput()->isKeyPressed('R'))
		mText->shadow = !mText->shadow;

	if (appInput()->isKeyPressed('T'))
		mText->border = !mText->border;

	if (appInput()->isKeyPressed('Y'))
		mText->gradient = !mText->gradient;
	

	if (appInput()->isKeyPressed('O'))
		mText->charactersDistCoef -= 0.1f;

	if (appInput()->isKeyPressed('P'))
		mText->charactersDistCoef += 0.1f;

	if (appInput()->isKeyPressed('K'))
		mText->linesDistCoef -= 0.1f;

	if (appInput()->isKeyPressed('L'))
		mText->linesDistCoef += 0.1f;
	
	
	if (appInput()->isKeyPressed(VK_NUMPAD4))
		mText->scale += vec2f(-0.1f, 0);
	
	if (appInput()->isKeyPressed(VK_NUMPAD6))
		mText->scale += vec2f(0.1f, 0);
	
	if (appInput()->isKeyPressed(VK_NUMPAD8))
		mText->scale += vec2f(0, -0.1f);
	
	if (appInput()->isKeyPressed(VK_NUMPAD2))
		mText->scale += vec2f(0, 0.1f);

	
	
	if (appInput()->isKeyPressed(VK_LEFT))
		mText->effectOffset += vec2f(-1, 0);
	
	if (appInput()->isKeyPressed(VK_RIGHT))
		mText->effectOffset += vec2f(1, 0);
	
	if (appInput()->isKeyPressed(VK_UP))
		mText->effectOffset += vec2f(0, -1);
	
	if (appInput()->isKeyPressed(VK_DOWN))
		mText->effectOffset += vec2f(0, 1);


	if (appInput()->isKeyPressed('A'))
		mText->horAlign = grText::HA_LEFT;

	if (appInput()->isKeyPressed('S'))
		mText->horAlign = grText::HA_CENTER;

	if (appInput()->isKeyPressed('D'))
		mText->horAlign = grText::HA_RIGHT;

	if (appInput()->isKeyPressed('F'))
		mText->horAlign = grText::HA_BOTH;


	if (appInput()->isKeyPressed('Z'))
		mText->verAlign = grText::VA_TOP;

	if (appInput()->isKeyPressed('X'))
		mText->verAlign = grText::VA_CENTER;

	if (appInput()->isKeyPressed('C'))
		mText->verAlign = grText::VA_BOTTOM;

	if (appInput()->isKeyPressed('V'))
		mText->verAlign = grText::VA_BOTH;
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