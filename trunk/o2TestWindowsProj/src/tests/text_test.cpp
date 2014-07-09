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
	rs->getFontManager()->loadBMFont("o2_font/myriad");
	mText = mnew grText(rs->getFontManager()->getFont("o2_font/myriad"));
	mText->ctext = "Prived, i'm super\nmany strings\ntext!!\nWith digits 1234567890\nAnd Looooooooooooooooooooooooooooooooooooooooooong liiiiiiiiiiiiiiiiineeeeeeeeeeeeeeeesssssssssssssss";
	//mText->ctext = "qwertyuioplkjhgfdsazxcvbnm1234567890";
	mText->rect = fRect(100, 100, 300, 300);
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
			mText->size -= appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 1) 
		{
			mText->size += vec2f(appInput()->getCursorDelta().x, -appInput()->getCursorDelta().y);
			mText->position += vec2f(0, appInput()->getCursorDelta().y);
		}		
		else if (mTouchCorner == 2) 
		{
			mText->size += appInput()->getCursorDelta();
		}
		else if (mTouchCorner == 3)
		{
			mText->position += vec2f(appInput()->getCursorDelta().x, 0);
			mText->size += vec2f(-appInput()->getCursorDelta().x, appInput()->getCursorDelta().y);
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

	
	if (appInput()->isKeyPressed('A'))
		mText->horAlign = grFont::HA_LEFT;

	if (appInput()->isKeyPressed('S'))
		mText->horAlign = grFont::HA_CENTER;

	if (appInput()->isKeyPressed('D'))
		mText->horAlign = grFont::HA_RIGHT;

	if (appInput()->isKeyPressed('F'))
		mText->horAlign = grFont::HA_BOTH;


	if (appInput()->isKeyPressed('Z'))
		mText->verAlign = grFont::VA_TOP;

	if (appInput()->isKeyPressed('X'))
		mText->verAlign = grFont::VA_CENTER;

	if (appInput()->isKeyPressed('C'))
		mText->verAlign = grFont::VA_BOTTOM;

	if (appInput()->isKeyPressed('V'))
		mText->verAlign = grFont::VA_BOTH;
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