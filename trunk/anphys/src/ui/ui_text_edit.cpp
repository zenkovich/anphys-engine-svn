#include "ui_text_edit.h"

#include "ui_font.h"
#include "ui_state.h"
#include "ui_manager.h"
#include "ui_label.h"
#include "util/other/string_utils.h"
#include "render/render_objects/2d/sprite.h"
#include "render/render_objects/2d/render_2d_object_mesh.h"

REGIST_TYPE(uiTextEdit)

uiTextEdit::uiTextEdit(uiWidgetsManager* widgetsManager, const std::string& id, uiFont* font, uiWidget* backWidget):
	uiWidget(widgetsManager, id), mFont(font), mBackWidget(backWidget), mSelectionMesh(NULL)
{
	addChild(mBackWidget);

	initialize();
}

uiTextEdit::uiTextEdit( const uiTextEdit& textWidget ):
	uiWidget(textWidget), mSelectionMesh(NULL)
{
	initialize();
}

uiTextEdit::~uiTextEdit()
{
	safe_release(mSelectedState);
	safe_release(mFocusedState);
	safe_release(mSelectionMesh);
	safe_release(mCaretSprite);
}

void uiTextEdit::derivedUpdate( float dt )
{
	vec2 newFontPosition = mGlobalPosition + mScrolling;
	if (newFontPosition != mLastFontPosition)
	{
		mFont->setPosition(newFontPosition);

		vec2 diff = newFontPosition - mLastFontPosition;
		for (int i = 0; i < (int)mSelectionMesh->mPolygonsCount; i += 2)
		{
			for (int j = 0; j < 4; j++)
			{
				mSelectionMesh->mVertexBuffer[i*2 + j].x += diff.x;
				mSelectionMesh->mVertexBuffer[i*2 + j].y += diff.y;
			}
		}

		mLastFontPosition = newFontPosition;
	}

	mCaretTimer += dt;
	if (mCaretTimer > mCaretFlashingPeriod)
		mCaretTimer -= mCaretFlashingPeriod;

	for (KeysList::iterator it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
	{
		it->mTimer += dt;

		if (it->mState == KeyDef::KS_PRESSED && it->mTimer > 0.5f)
		{
			it->mReady = true;
			it->mState = KeyDef::KS_DOWN;
			it->mTimer = 0;
		}

		if (it->mState == KeyDef::KS_DOWN && it->mTimer > 0.05f)
		{
			it->mReady = true;
			it->mTimer = 0;
		}
	}

	processInputKeys();

	for (KeysList::iterator it = mPressedKeys.begin(); it != mPressedKeys.end();)
	{
		if (it->mState == KeyDef::KS_RELEASED)
		{
			it = mPressedKeys.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void uiTextEdit::derivedDraw()
{
}

void uiTextEdit::afterDraw()
{
	mFont->draw();

	mSelectionMesh->draw();

	if (mFocused && mFirstCaretPos >= 0 && mFirstCaretPos == mSecondCaretPos)
	{
		if (mCaretTimer < mCaretFlashingPeriod*0.5f)
		{
			mCaretSprite->draw();
		}
	}

	if (mNeedUpdateSelectionMesh)
		updateSelectionMesh();
}

int uiTextEdit::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;
	if (!mVisible) return res;

	if (!mPressed && !message.isKeyDown(CURSOR_BUTTON))
	{
		if (mSelected && !isPointInside(message.mCursorPosition))
		{
			mSelected = false;
			mSelectedState->deactivate();
		}
		if (!mSelected && isPointInside(message.mCursorPosition))
		{
			mSelected = true;
			mSelectedState->activate();
			res = 1;
		}
	}

	if (message.isKeyPressed(CURSOR_BUTTON))
	{
		if (isPointInside(message.mCursorPosition))
		{
			pressedInside(message.mCursorPosition);
		}
		else
		{
			pressedOutside(message.mCursorPosition);
		}
	}

	if (mPressed && mLastCursorPoint != message.mCursorPosition)
	{
		mLastCursorPoint = message.mCursorPosition;
		cursorMoved(message.mCursorPosition);
	}

	if (message.isKeyReleased(CURSOR_BUTTON))
	{
		cursorReleased(message.mCursorPosition);
	}

	if (mFocused)
	{
		for (cInputMessage::IntArray::const_iterator it = message.mPressedKeys.cbegin();
			it != message.mPressedKeys.cend(); ++it)
		{
			registPressedKey(*it);
		}

		for (cInputMessage::IntArray::const_iterator it = message.mReleasedKeys.cbegin();
			it != message.mReleasedKeys.cend(); ++it)
		{
			unregistPressedKey(*it);
		}
	}

	if (mFocused)
		res = 1;

	return res;
}

uiTextEdit& uiTextEdit::setText( const std::string& text )
{
	mFont->setText(text);

	return *this;
}

wstring& uiTextEdit::getWText( bool textWillChange /*= true*/ )
{
	return mFont->getWText(textWillChange);
}

std::string uiTextEdit::getText()
{
	return mFont->getText();
}

uiTextEdit* uiTextEdit::clone()
{
	return new uiTextEdit(*this);
}

void uiTextEdit::onFocused()
{
	mFocusedState->activate();
}

void uiTextEdit::onUnfocused()
{
	mFocusedState->deactivate();
}

void uiTextEdit::initialize()
{
	mSelectedState = new uiState(this, "selected");
	mFocusedState = new uiState(this, "focused");

	mSelected = false;
	mPressed = false;

	mFirstCaretPos = 0;
	mSecondCaretPos = 0;
	mCaretTimer = 0;
	mCaretFlashingPeriod = 1.0f;

	mCaretSprite = new grSprite(mWidgetsManager->mRender);
	mCaretSprite->setColor(color4(0, 0, 0, 255));

	mMaxSelectionPolyCount = 128;
	mSelectionMesh = new grRender2DObjectMesh(mWidgetsManager->mRender, mMaxSelectionPolyCount*2, mMaxSelectionPolyCount);

	unsigned long selectionColor = color4(60, 60, 255, 100).dwordARGB();
	for (int i = 0; i < mMaxSelectionPolyCount; i += 2)
	{
		mSelectionMesh->mPolygonsBuffer[i]     = poly3(i*2, i*2 + 1, i*2 + 2);
		mSelectionMesh->mPolygonsBuffer[i + 1] = poly3(i*2, i*2 + 2, i*2 + 3);

		for (int j = 0; j < 4; j++)
			mSelectionMesh->mVertexBuffer[i*2 + j].color = selectionColor;
	}

	mFont->setHorAlign(uiFont::AL_LEFT);
	mFont->setVerAlign(uiFont::AL_TOP);

	mNeedUpdateSelectionMesh = false;
}

void uiTextEdit::pressedInside( const vec2& point )
{
	mPressed = true;

	mWidgetsManager->setWidgetFocused(this);

	mFirstCaretPos = mSecondCaretPos = getCaretByScreenCoords(point);

	mNeedUpdateSelectionMesh = true;

	mFocusedState->activate();
}

void uiTextEdit::pressedOutside( const vec2& point )
{
	mWidgetsManager->unfocusWidget(this);

	mFocusedState->deactivate();

	mSelected = false;
	mSelectedState->deactivate();
}

void uiTextEdit::cursorReleased( const vec2& point )
{
	mPressed = false;
}

void uiTextEdit::cursorMoved( const vec2& point )
{
	mSecondCaretPos = getCaretByScreenCoords(point);

	mNeedUpdateSelectionMesh = true;
}

int uiTextEdit::getCaretByScreenCoords( const vec2& point )
{
	/*int lineIdx = 0;
	int lastLineIdx = mFont->mCachedLines.size() - 1;

	for (uiFont::CacheLinesList::iterator it = mFont->mCachedLines.begin(); 
		 it != mFont->mCachedLines.end(); ++it, lineIdx++)
	{
		if ( ( (lineIdx == 0) ? false:(point.y < it->mRect.leftTop.y) ) || 
			 ( (lineIdx == lastLineIdx) ? false:(point.y > it->mRect.rightDown.y) ))
		{
			continue;
		}

		int charIdx = 0;
		int lastCharIdx = it->mCharactersGeometry.size() - 1;

		for (uiFont::RectsList::iterator jt = it->mCharactersGeometry.begin();
			 jt != it->mCharactersGeometry.end(); ++jt, charIdx++)
		{
			if ( ( (lineIdx == 0) ? false:(point.y < jt->leftTop.y) ) || 
				 ( (lineIdx == lastLineIdx) ? false:(point.y > jt->rightDown.y) ) ||
				 ( (charIdx == 0) ? false:(point.x < jt->leftTop.x) ) ||
				 ( (charIdx == lastCharIdx) ? false:(point.x > jt->rightDown.x) ))
			{
				continue;
			}

			int charid = it->mStartSymbol + charIdx;
			if (point.x > (jt->leftTop.x + jt->rightDown.x)*0.5f && charIdx != lastCharIdx)
			{
				return charid + 1;
			}

			return charid;
		}
	}*/

	return -1;
}

void uiTextEdit::updateSelectionMesh()
{
	mNeedUpdateSelectionMesh = false;

	if (!mSelectionMesh)
		return;

	mSelectionMesh->mPolygonsCount = 0;

	if (mFirstCaretPos < 0)
	{		
		return;
	}

	gLog->fout(1, "fisrt %i second %i\n", mFirstCaretPos, mSecondCaretPos);

	unsigned int minCaret = mFirstCaretPos;
	unsigned int maxCaret = mSecondCaretPos;
	bool invertedCaret = false;
	if (minCaret > maxCaret)
	{
		minCaret = mSecondCaretPos;
		maxCaret = mFirstCaretPos;
		invertedCaret = true;
	}
	
	bool inSelection = false;
	fRect currentSelectionLineRect;
	/*for (uiFont::CacheLinesList::iterator it = mFont->mCachedLines.begin(); it != mFont->mCachedLines.end(); ++it)
	{
		if (it->mCharactersGeometry.size() == 0)
			continue;

		if (inSelection)
		{
			currentSelectionLineRect.leftTop = it->mCharactersGeometry[0].leftTop;
		}

		if (minCaret >= it->mStartSymbol && minCaret <= it->mEndSymbol)
		{
			unsigned int symbolId = minCaret - it->mStartSymbol;
			fRect symbolRect = it->mCharactersGeometry[imin(symbolId, it->mCharactersGeometry.size() - 1)];

			vec2 leftTop = symbolRect.leftTop;
			/*if (symbolId == it->mCharactersGeometry.size())
				leftTop = vec2(symbolRect.rightDown.x, symbolRect.leftTop.y);*/

			/*if (invertedCaret)
			{
				mCaretSymbolSize = symbolRect.getSizeY();
				mCaretPos = leftTop + vec2(0, mCaretSymbolSize*0.5f);
			}

			if (minCaret == maxCaret)
			{
				mCaretSprite->setPosition(leftTop);
				mCaretSprite->setSize(vec2(2.0f, symbolRect.getSizeY()));
				mCaretTimer = 0;

				mCaretSymbolSize = symbolRect.getSizeY();
				mCaretPos = leftTop + vec2(0, mCaretSymbolSize*0.5f);

				return;
			}

			currentSelectionLineRect.leftTop = leftTop;
			inSelection = true;
		}

		if (maxCaret >= it->mStartSymbol && maxCaret <= it->mEndSymbol)
		{
			unsigned int symbolId = maxCaret - it->mStartSymbol;
			fRect symbolRect = it->mCharactersGeometry[imin(symbolId, it->mCharactersGeometry.size() - 1)];

			vec2 leftTop = symbolRect.leftTop;
			/*if (symbolId == it->mCharactersGeometry.size())
				leftTop = vec2(symbolRect.rightDown.x, symbolRect.leftTop.y);*/

			/*if (!invertedCaret)
			{
				mCaretSymbolSize = symbolRect.getSizeY();
				mCaretPos = leftTop + vec2(0, mCaretSymbolSize*0.5f);
			}

			currentSelectionLineRect.rightDown.x = leftTop.x;
			currentSelectionLineRect.rightDown.y = symbolRect.rightDown.y;

			pushSelectionRect(currentSelectionLineRect);
			break;
		}

		if (inSelection)
		{
			currentSelectionLineRect.rightDown = it->mCharactersGeometry.back().rightDown;
			pushSelectionRect(currentSelectionLineRect);
		}
	}*/
}

void uiTextEdit::pushSelectionRect( const fRect& rect )
{
	unsigned int firstIdx = mSelectionMesh->mPolygonsCount*2;

	mSelectionMesh->mVertexBuffer[firstIdx].x = rect.leftTop.x;
	mSelectionMesh->mVertexBuffer[firstIdx].y = rect.leftTop.y;

	mSelectionMesh->mVertexBuffer[firstIdx + 1].x = rect.rightDown.x;
	mSelectionMesh->mVertexBuffer[firstIdx + 1].y = rect.leftTop.y;

	mSelectionMesh->mVertexBuffer[firstIdx + 2].x = rect.rightDown.x;
	mSelectionMesh->mVertexBuffer[firstIdx + 2].y = rect.rightDown.y;

	mSelectionMesh->mVertexBuffer[firstIdx + 3].x = rect.leftTop.x;
	mSelectionMesh->mVertexBuffer[firstIdx + 3].y = rect.rightDown.y;

	mSelectionMesh->mPolygonsCount += 2;
}

void uiTextEdit::processInputKeys()
{
	wstring* text = &mFont->getWText();

	bool shift = false;
	bool ctrl  = false;

	for (KeysList::iterator it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
	{
		if (it->mKeyId == key_shift)
			shift = true;

		if (it->mKeyId == key_ctrl)
			ctrl = true;
	}

	int textLength = text->length();

	for (KeysList::iterator it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
	{
		if (!it->mReady)
			return;

		int key = it->mKeyId;
		it->mReady = false;

		if (key == key_left || key == key_right)
		{
			int offs = 0;

			if (key == key_left)
				offs = -1;
			else
				offs = 1;

			if (offs == 0)
				continue;

			if (shift)
			{
				mSecondCaretPos += offs;
			}
			else
			{
				if (mFirstCaretPos == mSecondCaretPos)
				{
					mFirstCaretPos += offs;

					if (mFirstCaretPos < 0)
						mFirstCaretPos = 0;


					if (mFirstCaretPos > textLength)
						mFirstCaretPos = textLength;

					mSecondCaretPos = mFirstCaretPos;
				}
				else
				{
					if (key == key_left)
						mFirstCaretPos = mSecondCaretPos = imin(mFirstCaretPos, mSecondCaretPos);
					else
						mFirstCaretPos = mSecondCaretPos = imax(mFirstCaretPos, mSecondCaretPos);
				}
			}

			mNeedUpdateSelectionMesh = true;

			continue;
		}

		if (key == key_up || key == key_down)
		{
			vec2 checkPoint = mCaretPos;

			if (key == key_up)
				checkPoint.y -= mCaretSymbolSize;
			else
				checkPoint.y += mCaretSymbolSize;

			int newCursorPos = getCaretByScreenCoords(checkPoint);

			if (shift)
			{
				mSecondCaretPos = newCursorPos;
			}
			else
			{
				mFirstCaretPos = mSecondCaretPos = newCursorPos;
			}

			mNeedUpdateSelectionMesh = true;

			continue;
		}

		if (key == key_del || key == key_backspace)
		{
			if (mFirstCaretPos == mSecondCaretPos)
			{
				if (key == key_del)
				{
					text->erase(text->begin() + mFirstCaretPos);
				}
				else
				{
					text->erase(text->begin() + mFirstCaretPos - 1);
					mFirstCaretPos = mSecondCaretPos = mFirstCaretPos - 1;
				}
			}
			else
			{
				text->erase(text->begin() + imin(mFirstCaretPos, mSecondCaretPos), 
					text->begin() + imax(mSecondCaretPos, mFirstCaretPos));

				mFirstCaretPos = mSecondCaretPos = imin(mFirstCaretPos, mSecondCaretPos);
			}

			mNeedUpdateSelectionMesh = true;
			continue;
		}

		char16_t character = (char16_t)getUnicodeFromVirtualCode(key);

		if (character == 0)
			continue;

		gLog->fout(1, "char %i %c\n", character, character);
		/*if (!message.isCharacter(character))
			continue;*/

		if (character == 13)
			character = 10;
			
		text->erase(text->begin() + imin(mFirstCaretPos, mSecondCaretPos), 
			text->begin() + imax(mSecondCaretPos, mFirstCaretPos));

		mFirstCaretPos = mSecondCaretPos = imin(mFirstCaretPos, mSecondCaretPos);

		text->insert(text->begin() + mFirstCaretPos, character);

		mNeedUpdateSelectionMesh = true;

		mFirstCaretPos += 1;
		mSecondCaretPos = mFirstCaretPos;
	}	
}

void uiTextEdit::registPressedKey( int key )
{
	mPressedKeys.push_back(KeyDef(key));
}

void uiTextEdit::unregistPressedKey( int key )
{
	for (KeysList::iterator it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
	{
		if (it->mKeyId == key)
		{
			it->mState = KeyDef::KS_RELEASED;
			return;
		}
	}
}
