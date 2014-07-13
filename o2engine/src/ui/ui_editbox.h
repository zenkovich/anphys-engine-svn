#ifndef UI_EDITBOX_H
#define UI_EDITBOX_H

#include "ui_drawables_list_widget.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grText;
class grFont;
class grSprite;
class grMesh;

class uiEditBox: public uiDrawablesListWidget
{
	friend class uiStdSkinInitializer;

	uiState*  mHoverState;         /** Hover state. */
	uiState*  mFocusedState;       /** Focused state. */
			 				       
	grText*   mText;               /** Text drawable. */
	float     mScrolling;          /** Scrolling in pixels. */
	cLayout   mClippingLayout;     /** Clipping area layout. */
	cLayout   mTextLayout;         /** Text layout. */
							       
	grSprite* mCursorSprite;       /** Cursor sprite drawable. */
	grMesh*   mSelectionMesh;      /** Selection mesh drawable. */
	color4    mSelectionColor;     
	int       mSelectionStart;     /** Selection start character idx. */
	int       mSelectionEnd;       /** Selection end character idx. */

	float     mCursorVisibleTimer; /** Cursor visible timer. When in greater than mCursorVisibleDelay, it will hide. */
	float     mCursorVisibleDelay; /** Cursor visible delay. */

public:
	DEFINE_TYPE(uiEditBox);

	PROPERTY(uiEditBox, string) ctext; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(uiEditBox, wstring) text; /** Caption property. Using set/getCaption. */

	cCallbackChain onTextChangeEvent;  /** On text changing event. */


	/** ctor. */
	uiEditBox(grFont* font, const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiEditBox(const uiEditBox& editbox);

	/** dtor. */
	~uiEditBox();

	/** Returns copy of widget. */
	uiWidget* clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

	/** Sets c text caption. */
	void setCText(const string& text);

	/** Returns c text caption. */
	string getCText() const;

	/** Sets caption text. */
	void setText(const wstring& text);

	/** Returns caption text. */
	wstring getText() const;

protected:
	/** Sets cursor color. */
	void setCursorColor(const color4& color);

	/** Sets selection color. */
	void setSelectionColor(const color4& color);

	/** Sets cursor visible state delay. */
	void setCursorVisibleDelay(float delay);

	/** Calls when added some state. */
	virtual void addedState(uiState* state);

	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);
	
	/** rocessing navigation. */
	void processNavigation( const cInputMessage &msg );

	/** Drawing current widget. */
	virtual void localDraw();

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();

	/** Returns character index at point. */
	int getCharacterIdxAtPoint(const vec2f& point);

	/** Returns character position by index. */
	vec2f getCharacterPosition(int idx);

	/** Adding rect to selection mesh. */
	void selectionAddRect(const fRect& rect);

	/** Drawing symbols debug frames. */
	void drawDebug();

	/** Updates cursor visible state. */
	void updateCursorVisible(float dt);

	/** Updates end selection position. */
	void updateSelectionEndPosition(int position);

	/** Updates selection mesh by selection start and end. */
	void updateSelection();

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_EDITBOX_H
