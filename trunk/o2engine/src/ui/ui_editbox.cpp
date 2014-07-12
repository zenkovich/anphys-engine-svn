#include "ui_editbox.h"

#include "render_system/text.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiEditBox);

uiEditBox::uiEditBox( grFont* font, const cLayout& layout, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHoverState(NULL), mFocusedState(NULL), mText(NULL), mCursorVisibleTimer(0.0f)
{
	mText = mnew grText(font);
	mText->setHorAlign(grFont::HA_LEFT);
	mText->setVerAlign(grFont::VA_CENTER);

	mCursorSprite = mnew grSprite();
	mCursorSprite->setSize(vec2f(1.0f, font->getLineHeight()));
	mCursorSprite->setRelativePivot(vec2f(0.0f, 1.0f));

	mSelectionColor = color4::green();
	mSelectionMesh = mnew grMesh(grTexture(), 4096, 2048);

	mCursorVisibleDelay = 0.7f;

	mSelectionStart = mSelectionEnd = 0;

	initializeProperties();
}

uiEditBox::uiEditBox( const uiEditBox& editbox ):
	uiDrawablesListWidget(editbox)
{
	mHoverState = getState("hover");
	mFocusedState = getState("focus");
	mText = mnew grText(*editbox.mText);
	mClippingLayout = editbox.mClippingLayout;
	mTextLayout = editbox.mTextLayout;	

	mCursorSprite = mnew grSprite();
	mCursorSprite->setSize(vec2f(1.0f, mText->getFont()->getLineHeight()));
	mCursorSprite->setRelativePivot(vec2f(0.0f, 1.0f));
	mCursorSprite->setColor(editbox.mCursorSprite->getColor());

	mSelectionColor = color4::green();
	mSelectionMesh = mnew grMesh(grTexture(), 4096, 2048);
	mSelectionColor = editbox.mSelectionColor;
	mCursorVisibleDelay = editbox.mCursorVisibleDelay;
	mCursorVisibleTimer = 0.0f;
	
	mSelectionStart = mSelectionEnd = 0;

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
}

bool uiEditBox::localProcessInputMessage( const cInputMessage& msg )
{
	if (msg.isCursorDown()) 
	{
		int charIdx = getCharacterIdxAtPoint(msg.getCursorPos());
		mSelectionEnd = charIdx;
		mCursorSprite->setPosition(getCharacterPosition(charIdx));
		mCursorSprite->setEnabled(true);
		mCursorVisibleTimer = 0;
		makeFocused();
	}

	return false;
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
	mTextLayout.update(mGlobalPosition, mSize);
	mText->setRect(mTextLayout.getRect());
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
		/*FOREACH(grFont::TextSymbolsSet::SymbolDefVec, line->mSymbols, symb) 
		{
			renderSystem()->drawRectFrame(symb->mFrame.getltCorner(), symb->mFrame.getrdCorner(), color4::someColor(idx++));
		}*/
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
		checkDown = lineIdx < symbSet->mLineDefs.size() - 1;

		int idx = 0;
		FOREACH(grFont::TextSymbolsSet::SymbolDefVec, line->mSymbols, symb) 
		{
			checkLeft = idx > 0;
			checkRight = idx < line->mSymbols.size() - 1;

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

	return -1;
}

vec2f uiEditBox::getCharacterPosition(int idx)
{
	if (idx < 0)
		return vec2f();

	grFont::TextSymbolsSet* symbSet = mText->getSymbolsSet();

	FOREACH(grFont::TextSymbolsSet::LineDefVec, symbSet->mLineDefs, line) 
	{
		if (line->mLineBegSymbol > idx)
			continue;

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

	return vec2f();
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

void uiEditBox::initializeProperties()
{
	REG_PROPERTY(uiEditBox, ctext, setCText, getCText);
	REG_PROPERTY(uiEditBox, text, setText, getText);
}

CLOSE_O2_NAMESPACE