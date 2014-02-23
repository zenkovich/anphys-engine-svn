#include "text_test.h"

#include "app/application.h"
#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/font_manager.h"

OPEN_O2_NAMESPACE

cTextTest::cTextTest( cApplication* app, cInputMessage* msg ):
	ITest(app, msg), mTouchCorner(-1)
{
	grRenderSystem* rs = mApplication->getRenderSystem();
	rs->getFontManager()->loadBMFont("arial");
	mText = mnew grText(rs, rs->getFontManager()->getFont("arial"));
}

void cTextTest::update( float dt )
{	
	vec2f corners[] = { mText->position, mText->position + vec2f(mText->areaSize.v().x, 0),
	                       mText->position + vec2f(0, mText->areaSize.v().y), mText->position + mText->areaSize };

}

void cTextTest::draw()
{
	mText->draw();

	//debug
	vec2f corners[] = { mText->position, mText->position + vec2f(mText->areaSize.v().x, 0),
	                       mText->position + vec2f(0, mText->areaSize.v().y), mText->position + mText->areaSize };

	grRenderSystem* rs = mApplication->getRenderSystem();
	rs->drawLine(corners[0], corners[1], color4::red().dword());
	rs->drawLine(corners[0], corners[3], color4::blue().dword());
	rs->drawLine(corners[1], corners[2], color4::white().dword());
	rs->drawLine(corners[3], corners[2], color4::white().dword());
}

CLOSE_O2_NAMESPACE