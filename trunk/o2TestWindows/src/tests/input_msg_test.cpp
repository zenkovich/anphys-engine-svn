#include "input_msg_test.h"

#include "app/application.h"
#include "render_system/font_manager.h"
#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "render_system/text.h"

OPEN_O2_NAMESPACE

InputMsgTest::InputMsgTest():
	mText(NULL)
{	
	grRenderSystem* rs = renderSystem();
	rs->getFontManager()->loadBMFont("arial");
	mText = mnew grText(rs->getFontManager()->getFont("arial"));
	mText->areaSize = vec2f(1000, 1000);
}

InputMsgTest::~InputMsgTest()
{
	safe_release(mText);
}

void InputMsgTest::draw()
{
	mText->draw();
}

void InputMsgTest::update( float dt )
{
	mText->ctext = "Input message:\n  Pressed keys: ";

	for (cInputMessage::KeysVec::iterator it = appInput()->mPressedKeys.begin(); 
		 it != appInput()->mPressedKeys.end(); ++it)
	{
		mText->ctext += format("%i (%.1f); ", it->mKey, it->mPressedTime);
	}

	mText->ctext += "\n  Down keys: ";

	for (cInputMessage::KeysVec::iterator it = appInput()->mDownKeys.begin(); 
		 it != appInput()->mDownKeys.end(); ++it)
	{
		mText->ctext += format("%i (%.1f); ", it->mKey, it->mPressedTime);
	}

	mText->ctext += "\n  Released keys: ";

	for (cInputMessage::KeysVec::iterator it = appInput()->mReleasedKeys.begin(); 
		 it != appInput()->mReleasedKeys.end(); ++it)
	{
		mText->ctext += format("%i (%.1f); ", it->mKey, it->mPressedTime);
	}

	mText->ctext += "\nCursors:\n";

	for (cInputMessage::CursorVec::iterator it = appInput()->mCursors.begin();
		 it != appInput()->mCursors.end(); ++it) 
	{
		mText->ctext += format("  %i (%i %i)->(%i %i)(%.1f);\n", it->mId, (int)it->mPosition.x, (int)it->mPosition.y,
			                   (int)it->mDelta.x, (int)it->mDelta.y, it->mPressedTime);
	}

	mText->ctext += "Released cursors: ";

	for (cInputMessage::CursorVec::iterator it = appInput()->mReleasedCursors.begin();
		 it != appInput()->mReleasedCursors.end(); ++it) 
	{
		mText->ctext += format("%i; ", it->mId);
	}

	mText->ctext += format("\nMain cursor pos (%i %i), delta (%i %i), alt (%s) alt2 (%s)",
		                   (int)appInput()->getCursorPos().x, (int)appInput()->getCursorPos().y,
						   (int)appInput()->getCursorDelta().x, (int)appInput()->getCursorDelta().y,
						   appInput()->isAltCursorDown() ? "down":"up",
						   appInput()->isAlt2CursorDown() ? "down":"up");
}

CLOSE_O2_NAMESPACE