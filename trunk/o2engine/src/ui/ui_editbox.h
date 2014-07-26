#ifndef UI_EDITBOX_H
#define UI_EDITBOX_H

#include "ui_drawables_list_widget.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grText;
class grFont;
class grSprite;
class grMesh;
class uiScrollBar;

class uiEditBox: public uiDrawablesListWidget
{
	friend class uiStdSkinInitializer;

	uiState*     mHoverState;           /** Hover state. */
	uiState*     mFocusedState;         /** Focused state. */
			  	  			       	    
	grText*      mText;                 /** Text drawable. */
	vec2f        mScroll;               /** Scrolling in pixels. */
	vec2f        mSmoothScroll;         /** Smoothed scrolling. */
	cLayout      mClippingLayout;       /** Clipping area layout. */
	cLayout      mTextLayout;           /** Text layout. */
				   			       	    
	grSprite*    mCursorSprite;         /** Cursor sprite drawable. */
	grMesh*      mSelectionMesh;        /** Selection mesh drawable. */
	color4       mSelectionColor;       
	int          mSelectionStart;       /** Selection start character idx. */
	int          mSelectionEnd;         /** Selection end character idx. */
			     					    
	float        mCursorVisibleTimer;   /** Cursor visible timer. When in greater than mCursorVisibleDelay, it will hide. */
	float        mCursorVisibleDelay;   /** Cursor visible delay. */
	bool         mMultiLine;            /** True, if multi line. */
			     
	bool         mCursorInsideClipping; /** True, if cursor inside clipping rect. */
	
	uiScrollBar* mHorScrollbar;         /** Horizontal scroll bar child widget. */
	uiScrollBar* mVerScrollbar;         /** Vertical scroll bar child widget. */


public:
	DEFINE_TYPE(uiEditBox);

	PROPERTY(uiEditBox, string) ctext; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(uiEditBox, wstring) text; /** Caption property. Using set/getCaption. */

	cCallbackChain onTextChangeEvent;  /** On text changing event. */


	/** ctor. */
	uiEditBox(grFont* font, const cLayout& layout, uiScrollBar* horBarSample = NULL, uiScrollBar* verBarSample = NULL, 
		      const string& id = "");

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

	/** Sets word wrapping. */
	void setWordWrap(bool wordWrap);

	/** returns word wrapping flag. */
	bool isWordWrap() const;

	/** Sets horizontal scroll bar sample. */
	void setHorScrollbar(uiScrollBar* scrollbar);

	/** Sets vertical scroll bar sample. */
	void setVerScrollbar(uiScrollBar* scrollbar);

	/** Sets current scroll. */
	void setScroll(const vec2f& scroll);

	/** Returns current scroll. */
	vec2f getScroll() const;

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

	/** Processing characters input. */
	void processInputCharacters(const cInputMessage &msg);

	/** Processing erasing. */
	void processErasing(const cInputMessage &msg);

	/** Processing navigation. */
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

	/** Jumping at direction to next jump symbol. */
	void jumpSelection(bool forward, bool selecting);

	/** Adding rect to selection mesh. */
	void selectionAddRect(const fRect& rect);

	/** Drawing symbols debug frames. */
	void drawDebug();

	/** Updates cursor visible state. */
	void updateCursorVisible(float dt);

	/** Updates end selection position. */
	void updateSelectionEndPosition(int position, bool selecting);

	/** Updates cursor sprite position by end selection index. */
	void updateCursorSpritePos();

	/** Updates selection mesh by selection start and end. */
	void updateSelection();

	/** Updates scrolling for visible cursor. */
	void checkScrollingToCursor();

	/** Calls when hor or vertical bar moved. */
	void scrollChanged();

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_EDITBOX_H
