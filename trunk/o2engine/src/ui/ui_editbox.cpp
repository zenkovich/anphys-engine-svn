#include "ui_editbox.h"

#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/mesh.h"
#include "ui_scroll_bar.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiEditBox);

uiEditBox::uiEditBox( grFont* font, const cLayout& layout, uiScrollBar* horBarSample /*= NULL*/, 
	                  uiScrollBar* verBarSample /*= NULL*/, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHoverState(NULL), mFocusedState(NULL), mText(NULL), mCursorVisibleTimer(0.0f),
	mMultiLine(false), mHorScrollbar(NULL), mVerScrollbar(NULL), mCursorInsideClipping(false)
{
	mText = mnew grText(font);
	mText->setHorAlign(grFont::HA_LEFT);
	mText->setVerAlign(grFont::VA_CENTER);

	mCursorSprite = mnew grSprite();
	mCursorSprite->setSize(vec2f(1.0f, font->getLineHeight()*mText->getLinesDistCoef()));
	mCursorSprite->setRelativePivot(vec2f(0.0f, 1.0f));

	mSelectionColor = color4::green();
	mSelectionMesh = mnew grMesh(grTexture(), 4096, 2048);

	mCursorVisibleDelay = 0.7f;

	mSelectionStart = mSelectionEnd = 0;
	setHorScrollbar(horBarSample);
	setVerScrollbar(verBarSample);

	initializeProperties();
}

uiEditBox::uiEditBox( const uiEditBox& editbox ):
	uiDrawablesListWidget(editbox), mMultiLine(false), mHorScrollbar(NULL), mVerScrollbar(NULL), mCursorInsideClipping(false)
{
	mHoverState = getState("hover");
	mFocusedState = getState("focus");
	mText = mnew grText(*editbox.mText);
	mClippingLayout = editbox.mClippingLayout;
	mTextLayout = editbox.mTextLayout;	
	mMultiLine = editbox.mMultiLine;

	mCursorSprite = mnew grSprite();
	mCursorSprite->setSize(vec2f(1.0f, mText->getFont()->getLineHeight()*mText->getLinesDistCoef()));
	mCursorSprite->setRelativePivot(vec2f(0.0f, 1.0f));
	mCursorSprite->setColor(editbox.mCursorSprite->getColor());

	mSelectionColor = color4::green();
	mSelectionMesh = mnew grMesh(grTexture(), 4096, 2048);
	mSelectionColor = editbox.mSelectionColor;
	mCursorVisibleDelay = editbox.mCursorVisibleDelay;
	mCursorVisibleTimer = 0.0f;
	
	mSelectionStart = mSelectionEnd = 0;

	if (editbox.mHorScrollbar)
		setHorScrollbar(getWidgetByType<uiScrollBar>(editbox.mHorScrollbar->getId()));
	else 
		setHorScrollbar(NULL);

	if (editbox.mVerScrollbar)
		setVerScrollbar(getWidgetByType<uiScrollBar>(editbox.mVerScrollbar->getId()));
	else
		setVerScrollbar(NULL);

	initializeProperties();
}

uiEditBox::~uiEditBox()
{
	safe_release(mText);
	safe_release(mCursorSprite);
	safe_release(mSelectionMesh);
}

uiWidget* uiEditBox::clone() const
{
	return mnew uiEditBox(*this);
}

bool uiEditBox::isFocusable() const
{
	return true;
}

void uiEditBox::setCText( const string& text )
{
	mText->setCText(text);
}

string uiEditBox::getCText() const
{
	return mText->getCText();
}

void uiEditBox::setText( const wstring& text )
{
	mText->setText(text);
}

wstring uiEditBox::getText() const
{
	return mText->getText();
}

void uiEditBox::setWordWrap( bool wordWrap )
{
	mText->setWordWrap(wordWrap);
	mText->forceUpdateMesh();
	updateSelectionEndPosition(mSelectionEnd, true);
}

bool uiEditBox::isWordWrap() const
{
	return mText->getWordWrap();
}

void uiEditBox::setHorScrollbar( uiScrollBar* scrollbar )
{
	if (mHorScrollbar)
		removeChild(mHorScrollbar);

	if (!scrollbar)
		return;

	addChild(scrollbar);
	mHorScrollbar = scrollbar;
	mHorScrollbar->onValueChangedEvent.add(callback<uiEditBox>(this, &uiEditBox::scrollChanged));
}

void uiEditBox::setVerScrollbar( uiScrollBar* scrollbar )
{
	if (mVerScrollbar)
		removeChild(mVerScrollbar);

	if (!scrollbar)
		return;
	
	addChild(scrollbar);
	mVerScrollbar = scrollbar;
	mVerScrollbar->onValueChangedEvent.add(callback<uiEditBox>(this, &uiEditBox::scrollChanged));
}

void uiEditBox::addedState( uiState* state )
{
	if (state->getName() == "hover")
		mHoverState = state;
	else if (state->getName() == "focus")
		mFocusedState = state;
}

void uiEditBox::localUpdate( float dt )
{
	if (mHoverState)
		mHoverState->setState(mCursorInside);

	updateCursorVisible(dt);

	//update scrolling
	float scrollingCoef = 20.0f;
	vec2f lastSmoothScroll = mSmoothScroll;
	mSmoothScroll = lerp(mSmoothScroll, mScroll, clamp(dt*scrollingCoef, 0.0f, 1.0f));

	if (!equals(lastSmoothScroll, mSmoothScroll)) {
		updateLayout();
	}
}

bool uiEditBox::localProcessInputMessage( const cInputMessage& msg )
{
	mCursorInsideClipping = mClippingLayout.getRect().isInside(msg.getCursorPos());	

	float scrollCoef = 0.1f;
	float mouseWheelDelta = msg.getMouseWheelDelta()*scrollCoef;
	if (!equals(mouseWheelDelta, 0.0f) && mCursorInside)
	{
		if (mVerScrollbar && mVerScrollbar->isVisible())
			mVerScrollbar->setValueClamped(mVerScrollbar->getValue() - mouseWheelDelta);
		else if (mHorScrollbar)
			mHorScrollbar->setValueClamped(mHorScrollbar->getValue() - mouseWheelDelta);
	}

	if (msg.isCursorPressed())
	{
		if (mCursorInside)			
			makeFocused();
		else
			releaseFocus();
	}

	processNavigation(msg);
	processErasing(msg);
	processInputCharacters(msg);	

	return false;
}

void uiEditBox::processNavigation( const cInputMessage &msg )
{
	if (msg.isCursorPressed() && mCursorInsideClipping)
	{
		if (mCursorInside)
		{
			int charIdx = getCharacterIdxAtPoint(msg.getCursorPos());
			updateSelectionEndPosition(charIdx, false);
			mSelectionStart = mSelectionEnd;
			updateSelection();
		}
	}

	if (msg.isCursorDown()) 
	{
		int charIdx = getCharacterIdxAtPoint(msg.getCursorPos());
		updateSelectionEndPosition(charIdx, true);
	}

	bool selecting = msg.isKeyDown(VK_SHIFT);
	if (msg.isKeyPressed(VK_LEFT) || msg.isKeyRepeating(VK_LEFT))		
	{
		if (msg.isKeyDown(VK_CONTROL))
			jumpSelection(false, selecting);
		else
			updateSelectionEndPosition(mSelectionEnd - 1, selecting);
	}

	if (msg.isKeyPressed(VK_RIGHT) || msg.isKeyRepeating(VK_RIGHT))		
	{
		if (msg.isKeyDown(VK_CONTROL))
			jumpSelection(true, selecting);
		else
			updateSelectionEndPosition(mSelectionEnd + 1, selecting);
	}

	if (msg.isKeyPressed(VK_UP) || msg.isKeyRepeating(VK_UP))
		updateSelectionEndPosition(getCharacterIdxAtPoint(getCharacterPosition(mSelectionEnd) - 
		                                                  vec2f(0.0f, mText->getFont()->getLineHeight()*1.5f)), selecting);

	if (msg.isKeyPressed(VK_DOWN) || msg.isKeyRepeating(VK_DOWN))
		updateSelectionEndPosition(getCharacterIdxAtPoint(getCharacterPosition(mSelectionEnd) + 
		                                                  vec2f(0.0f, mText->getFont()->getLineHeight()*0.5f)), selecting);

	if (msg.isKeyDown(VK_END))
	{
		wstring text = mText->getText();
		int endLineSymbol = mSelectionEnd;
		for (int i = mSelectionEnd; i < (int)text.length(); i++)
		{
			endLineSymbol = i + 1;
			if (text[i] == '\n') 
			{
				endLineSymbol--;
				break;
			}
		}

		updateSelectionEndPosition(endLineSymbol, selecting);
	}

	if (msg.isKeyDown(VK_HOME))
	{
		wstring text = mText->getText();
		int endLineSymbol = mSelectionEnd;
		for (int i = mSelectionEnd -1; i >= 0; i--)
		{
			endLineSymbol = i;
			if (text[i] == '\n') 
			{
				endLineSymbol++;
				break;
			}
		}

		updateSelectionEndPosition(endLineSymbol, selecting);
	}
}

void uiEditBox::jumpSelection(bool forward, bool selecting)
{
	static char jumpSymbols[] = " \n()-=_+\\|/**&^%$#@!~,.?";
	int jumpSymbolsCount = strlen(jumpSymbols);

	wstring text = mText->getText();
	int jumpIdx = mSelectionEnd + 1;

	if (forward) 
	{
		bool stop = false;
		for (int i = mSelectionEnd; i < (int)text.length() && !stop; i++)
		{
			jumpIdx = i;
			for (int j = 0; j < jumpSymbolsCount; j++)
			{
				if (text[i] == jumpSymbols[j])
				{
					stop = true;
					break;
				}
			}
		}

		if (!stop) jumpIdx++;
	}
	else
	{		
		bool stop = false;
		for (int i = min(mSelectionEnd - 1, (int)text.length() - 1); i >= 0 && !stop; i--)
		{
			jumpIdx = i;
			for (int j = 0; j < jumpSymbolsCount; j++)
			{
				if (text[i] == jumpSymbols[j])
				{
					jumpIdx++;
					stop = true;
					break;
				}
			}
		}

		if (!stop) jumpIdx--;
	}

	if (jumpIdx == mSelectionEnd)
	{
		if (forward)
			jumpIdx++;
		else
			jumpIdx--;
	}

	updateSelectionEndPosition(jumpIdx, selecting);
}

void uiEditBox::processErasing(const cInputMessage &msg)
{
	if (msg.isKeyPressed(VK_BACK) || msg.isKeyRepeating(VK_BACK))
	{
		if (mSelectionEnd > 0) 
		{
			wstring text = mText->getText();

			if (mSelectionEnd == mSelectionStart)
				text.erase(mSelectionEnd - 1, 1);
			else
				text.erase(min(mSelectionStart, mSelectionEnd), abs(mSelectionEnd - mSelectionStart));

			mText->setText(text);
			mText->forceUpdateMesh();		
			
			if (mSelectionEnd == mSelectionStart)
				updateSelectionEndPosition(mSelectionEnd -1, false);
			else
				updateSelectionEndPosition(min(mSelectionEnd, mSelectionStart), false);

			layoutUpdated();
			checkScrollingToCursor();
		}
	}

	if (msg.isKeyPressed(VK_DELETE) || msg.isKeyRepeating(VK_DELETE))
	{
		if (mSelectionEnd < (int)mText->getText().length() + 1) 
		{
			wstring text = mText->getText();
			
			if (mSelectionEnd == mSelectionStart)
				text.erase(mSelectionEnd, 1);
			else
				text.erase(min(mSelectionStart, mSelectionEnd), abs(mSelectionEnd - mSelectionStart));

			mText->setText(text);
			mText->forceUpdateMesh();		
			updateSelectionEndPosition(min(mSelectionEnd, mSelectionStart), false);

			layoutUpdated();
			checkScrollingToCursor();
		}
	}
}

void uiEditBox::processInputCharacters(const cInputMessage &msg)
{
	cInputMessage::KeysVec pressedKeys = msg.getPressedKeys();
	cInputMessage::KeysVec downKeys = msg.getDownKeys();

	FOREACH(cInputMessage::KeysVec, downKeys, key) 
	{
		if (msg.isKeyRepeating(key->mKey))
			pressedKeys.push_back(*key);
	}

	FOREACH(cInputMessage::KeysVec, pressedKeys, key) 
	{
		char16_t ch = (char16_t)getUnicodeFromVirtualCode(key->mKey);
		hlog("char %i", (int)ch);
		if (ch != 0 && ch != 8)
		{
			if (ch == 13)
				ch = 10;

			if (ch == 10 && !mMultiLine)
				continue;

			wstring text = mText->getText();

			if (mSelectionStart != mSelectionEnd)
			{
				text.erase(min(mSelectionStart, mSelectionEnd), abs(mSelectionEnd - mSelectionStart));
				mSelectionEnd = min(mSelectionStart, mSelectionEnd);
				mSelectionStart = mSelectionEnd;
			}

			text.insert(text.begin() + mSelectionEnd, ch);
			mText->setText(text);
			mText->forceUpdateMesh();
			updateSelectionEndPosition(mSelectionEnd + 1, false);
			mSelectionStart = mSelectionEnd;
			hlog("char %i", (int)ch);

			layoutUpdated();		
			checkScrollingToCursor();
		}
	}
}

void uiEditBox::localDraw()
{
	uiDrawablesListWidget::localDraw();

	renderSystem()->setupScissorRect(mClippingLayout.getRect());
	renderSystem()->enableScissorTest();

	mText->draw();
	mSelectionMesh->draw();
	mCursorSprite->draw();
	//drawDebug();

	renderSystem()->disableScissorTest();
}

void uiEditBox::layoutUpdated()
{
	uiDrawablesListWidget::layoutUpdated();

	mClippingLayout.update(mGlobalPosition, mSize);
	mTextLayout.update(mGlobalPosition - mSmoothScroll, mSize);
	mText->setRect(mTextLayout.getRect());
	mText->forceUpdateMesh();

	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();
	fRect contentSize(mTextLayout.mPosition, symbSet->mPosition + symbSet->mRealSize + vec2f(5.0f, 5.0f));

	if (mHorScrollbar)
	{
		mHorScrollbar->setValueRange(0.0f, max(contentSize.getSizeX() - mClippingLayout.mSize.x, 0.1f));
		mHorScrollbar->setBarSize(mClippingLayout.mSize.x/contentSize.getSizeX()*mHorScrollbar->getMaxValue());
		mHorScrollbar->setVisible(mHorScrollbar->getBarSize() < mHorScrollbar->getMaxValue() - mHorScrollbar->getMinValue());
		if (!mHorScrollbar->isVisible()) 
		{
			mScroll.x = 0;
		}
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->setValueRange(0.0f, max(contentSize.getSizeY() - mClippingLayout.mSize.y, 0.1f));
		mVerScrollbar->setBarSize(mClippingLayout.mSize.y/contentSize.getSizeY()*mVerScrollbar->getMaxValue());
		mVerScrollbar->setVisible(mVerScrollbar->getBarSize() < mVerScrollbar->getMaxValue() - mVerScrollbar->getMinValue());
		if (!mVerScrollbar->isVisible()) 
			mScroll.y = 0;
	}
	
	updateCursorSpritePos();
	updateSelection();
}

void uiEditBox::onFocused()
{
	uiWidget::onFocused();

	if (mFocusedState)
		mFocusedState->setState(true);

	mCursorSprite->setEnabled(true);
	mCursorVisibleTimer = 0;
}

void uiEditBox::onFocusLost()
{
	uiWidget::onFocusLost();

	if (mFocusedState)
		mFocusedState->setState(false);

	mCursorSprite->setEnabled(false);
	mCursorVisibleTimer = 0;
}

void uiEditBox::drawDebug()
{
	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();

	int idx = 0;
	FOREACH(grFont::TextSymbolsSet::LineDefVec, symbSet->mLineDefs, line) 
	{
		renderSystem()->drawRectFrame(line->mPosition, line->mPosition + line->mSize);
		FOREACH(grFont::TextSymbolsSet::SymbolDefVec, line->mSymbols, symb) 
		{
			renderSystem()->drawRectFrame(symb->mFrame.getltCorner(), symb->mFrame.getrdCorner(), color4::someColor(idx++));
		}
	}
}

int uiEditBox::getCharacterIdxAtPoint(const vec2f& point)
{
	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();


	bool checkUp, checkDown, checkLeft, checkRight;
	int lineIdx = 0;
	FOREACH(grFont::TextSymbolsSet::LineDefVec, symbSet->mLineDefs, line) 
	{
		checkUp = lineIdx > 0;
		checkDown = lineIdx < (int)symbSet->mLineDefs.size() - 1;

		if (line->mSymbols.size() == 0 && point.y > line->mPosition.y && point.y < line->mPosition.y + line->mSize.y)
			return line->mLineBegSymbol;

		int idx = 0;
		FOREACH(grFont::TextSymbolsSet::SymbolDefVec, line->mSymbols, symb) 
		{
			checkLeft = idx > 0;
			checkRight = idx < (int)line->mSymbols.size() - 1;

			fRect sr = symb->mFrame;

			bool ls = checkLeft ? sr.left < point.x:true;
			bool rs = checkRight ? sr.right > point.x:true;
			bool ts = checkUp ? sr.top < point.y:true;
			bool bs = checkDown ? sr.down > point.y:true;

			if (ls && rs && ts && bs)
			{
				if (point.x > (symb->mFrame.left + symb->mFrame.right)*0.5f)
					return line->mLineBegSymbol + idx + 1;
				else
					return line->mLineBegSymbol + idx;
			}

			idx++;
		}

		lineIdx++;
	}

	return 0;
}

vec2f uiEditBox::getCharacterPosition(int idx)
{
	if (idx < 0)
		return vec2f();

	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();

	FOREACH(grFont::TextSymbolsSet::LineDefVec, symbSet->mLineDefs, line) 
	{
		if (line->mLineBegSymbol + (int)line->mSymbols.size() < idx)
			continue;

		if (line->mSymbols.size() == 0)
			return line->mPosition + vec2f(0.0f, line->mSize.y);

		int locIdx = idx - line->mLineBegSymbol;
		vec2f res;
		if (locIdx >= (int)line->mSymbols.size()) 
		{
			grFont::TextSymbolsSet::symbolDef* symb = &(line->mSymbols[line->mSymbols.size() - 1]);
			res = symb->mFrame.getldCorner() - symb->mOffset + vec2f(symb->mAdvance, 0.0f);
		}
		else 
		{
			grFont::TextSymbolsSet::symbolDef* symb = &(line->mSymbols[locIdx]);
			res = symb->mFrame.getldCorner() - symb->mOffset;
		}
		res.y = line->mPosition.y + line->mSize.y;

		return res;
	}

	return mTextLayout.mPosition + vec2f(0.0f, mText->getFont()->getLineHeight()*(1.0f + 1.0f - mText->getLinesDistCoef()));
}

void uiEditBox::setCursorColor(const color4& color)
{
	mCursorSprite->setColor(color);
}

void uiEditBox::setSelectionColor(const color4& color)
{
	mSelectionColor = color;
}

void uiEditBox::setCursorVisibleDelay(float delay)
{
	mCursorVisibleDelay = delay;
}

void uiEditBox::selectionAddRect(const fRect& rect)
{
	unsigned long color = mSelectionColor.dword();
	
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = vertex2(rect.getltCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = vertex2(rect.getrtCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = vertex2(rect.getrdCorner(), color, 0.0f, 0.0f);
	mSelectionMesh->mVerticies[mSelectionMesh->mVertexCount++] = vertex2(rect.getldCorner(), color, 0.0f, 0.0f);
	
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3    ] = mSelectionMesh->mVertexCount - 4;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 1] = mSelectionMesh->mVertexCount - 3;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 2] = mSelectionMesh->mVertexCount - 2;
	mSelectionMesh->mPolyCount++;
	
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3    ] = mSelectionMesh->mVertexCount - 4;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 1] = mSelectionMesh->mVertexCount - 2;
	mSelectionMesh->mIndexes[mSelectionMesh->mPolyCount*3 + 2] = mSelectionMesh->mVertexCount - 1;
	mSelectionMesh->mPolyCount++;
}

void uiEditBox::updateCursorVisible(float dt)
{
	mCursorVisibleTimer += dt;
	if (mCursorVisibleTimer > mCursorVisibleDelay) 
	{
		mCursorVisibleTimer = 0.0f;
		mCursorSprite->setEnabled(!mCursorSprite->isEnabled());
	}

	mCursorSprite->setEnabled(mCursorSprite->isEnabled() && mSelectionEnd >= 0 && mFocused);
}

void uiEditBox::updateSelectionEndPosition( int position, bool selecting )
{
	mSelectionEnd = clamp(position, 0, (int)mText->getText().length());

	updateCursorSpritePos();

	if (!selecting) 
		mSelectionStart = mSelectionEnd;

	updateSelection();
	checkScrollingToCursor();
}

void uiEditBox::updateCursorSpritePos()
{
	mCursorSprite->setPosition(getCharacterPosition(mSelectionEnd));
	mCursorSprite->setEnabled(true);
	mCursorVisibleTimer = 0;
}

void uiEditBox::updateSelection()
{
	mSelectionMesh->mVertexCount = 0;
	mSelectionMesh->mPolyCount = 0;

	if (mSelectionStart == mSelectionEnd)
		return;
	
	int start = min(mSelectionStart, mSelectionEnd);
	int end = max(mSelectionStart, mSelectionEnd);

	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();

	FOREACH(grFont::TextSymbolsSet::LineDefVec, symbSet->mLineDefs, line) 
	{
		if (start > line->mLineBegSymbol + (int)line->mSymbols.size() || end < line->mLineBegSymbol)
			continue;

		if (line->mSymbols.size() == 0)
		{
			grFont* font = mText->getFont();
			vec2f lt = line->mPosition + vec2f(0, font->getLineHeight()*(1.0f - mText->getLinesDistCoef()));
			vec2f rd = lt + vec2f(font->getSymbolAdvance(' '), font->getLineHeight()*mText->getLinesDistCoef());
			selectionAddRect(fRect(lt, rd));

			continue;
		}

		int begSymbol = max(start - line->mLineBegSymbol, 0);
		int endSymbol = end - line->mLineBegSymbol;
		float endOffs = 0, begOffs = 0;

		if (begSymbol >= (int)line->mSymbols.size())
		{
			begOffs = line->mSymbols.back().mAdvance;
			begSymbol = line->mSymbols.size() - 1;
		}

		if (endSymbol >= (int)line->mSymbols.size()) 
		{
			endOffs = line->mSymbols.back().mAdvance;
			if (line->mEndedNewLine && endSymbol > (int)line->mSymbols.size())
				endOffs += mText->getFont()->getSymbolAdvance(' ');

			endSymbol = line->mSymbols.size() - 1;
		}

		vec2f lt(line->mSymbols[begSymbol].mFrame.left - line->mSymbols[begSymbol].mOffset.x + begOffs, 
			     line->mPosition.y + line->mSize.y*(1.0f - mText->getLinesDistCoef()));
		vec2f rd(line->mSymbols[endSymbol].mFrame.left - line->mSymbols[endSymbol].mOffset.x + endOffs, 
			     line->mPosition.y + line->mSize.y);

		selectionAddRect(fRect(lt, rd));
	}
}

void uiEditBox::scrollChanged()
{
	if (mHorScrollbar)
		mScroll.x = mHorScrollbar->getValue();

	if (mVerScrollbar)
		mScroll.y = mVerScrollbar->getValue();

	updateLayout();
}

void uiEditBox::checkScrollingToCursor()
{
	vec2f cursorPos = mCursorSprite->getPosition();
	fRect clipRect = mClippingLayout.getRect();
	
	float rightOffs = max(cursorPos.x - clipRect.right + 5.0f, 0.0f);
	float leftOffs = max(clipRect.left - cursorPos.x + 5.0f, 0.0f);


	float downOffs = max(cursorPos.y - clipRect.down + 5.0f, 0.0f);
	float topOffs = max(clipRect.top - cursorPos.y + 5.0f + mText->getFont()->getLineHeight(), 0.0f);
	
	if (mHorScrollbar)
		mHorScrollbar->setValueClamped(mScroll.x + rightOffs - leftOffs);
	else
	{
		mScroll.x = clamp(mScroll.x + rightOffs - leftOffs, 
			              0.0f, max(mText->getSymbolsSet()->mRealSize.x - clipRect.getSizeX(), 0.0f) + 5.0f);
	}

	if (mMultiLine)
	{
		if (mVerScrollbar)
			mVerScrollbar->setValueClamped(mScroll.y + downOffs - topOffs);
		else
			mScroll.y = mScroll.y + downOffs - topOffs;
	}
}

void uiEditBox::initializeProperties()
{
	REG_PROPERTY(uiEditBox, ctext, setCText, getCText);
	REG_PROPERTY(uiEditBox, text, setText, getText);
}

CLOSE_O2_NAMESPACE