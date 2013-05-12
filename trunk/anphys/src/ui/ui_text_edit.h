#ifndef UI_TEXT_EDIT_H
#define UI_TEXT_EDIT_H

#include "ui_widget.h"
#include "ui_binding_values.h"

struct uiFont;
struct cCallbackInterface;
struct grSprite;
struct grRender2DObjectMesh;

struct uiTextEdit:public uiWidget, public uiBindingValues
{
	DEFINE_TYPE(uiTextEdit)

	struct KeyDef
	{
		enum State { KS_PRESSED = 0, KS_DOWN, KS_RELEASED };

		int   mKeyId;
		float mTimer;
		State mState;
		bool  mReady;

		KeyDef():mKeyId(0), mTimer(0), mState(KS_PRESSED), mReady(true) {}
		KeyDef(int key):mKeyId(key), mTimer(0), mState(KS_PRESSED), mReady(true) {}
	};
	typedef std::vector<KeyDef> KeysList;

	uiState*              mSelectedState;
	uiState*              mFocusedState;
						  
private:				  
	vec2                  mLastFontPosition;
	vec2                  mLastCursorPoint;
						  
	cCallbackInterface*   mOnTextChangedCallback;
						  
	bool                  mSelected;
	bool                  mPressed;
						  
	uiFont*               mFont;
	uiWidget*             mBackWidget;
						  
	int                   mFirstCaretPos;
	int                   mSecondCaretPos;

	grRender2DObjectMesh* mSelectionMesh;
	grSprite*             mCaretSprite;
	float                 mCaretTimer;
	vec2                  mCaretPos;
	float                 mCaretSymbolSize;

	vec2                  mScrolling;
	vec2                  mTextOffset;

	int                   mMaxSelectionPolyCount;
	float                 mCaretFlashingPeriod;

	bool                  mNeedUpdateSelectionMesh;

	KeysList              mPressedKeys;

	bool                  mSingeLine;

public:
	uiTextEdit(uiWidgetsManager* widgetsManager, const std::string& id, uiFont* font, uiWidget* backWidget);
	uiTextEdit(const uiTextEdit& textWidget);
	~uiTextEdit();

	void        derivedUpdate(float dt);

	void        derivedDraw();
	void        afterDraw();

	int         processInputMessageDerived(const cInputMessage& message);

	void        onFocused();
	void        onUnfocused();

	uiTextEdit& setText(const std::string& text);
	std::string getText();
	wstring&    getWText(bool textWillChange = true);

	uiTextEdit& setTextOffset(const vec2& offset);
	vec2        getTextOffset() const;

	uiTextEdit& setSingleLine(bool singleLine);
	bool        isSingleLine() const;

	uiTextEdit* clone();

	bool        isAdjustingSizeByChilds() { return false; }

private:
	void initialize();

	void pressedInside(const vec2& point);
	void pressedOutside(const vec2& point);
	void cursorReleased(const vec2& point);
	void cursorMoved(const vec2& point);

	int  getCaretByScreenCoords(const vec2& point);

	void updateSelectionMesh();
	void pushSelectionRect(const fRect& rect);

	void registPressedKey(int key);
	void unregistPressedKey(int key);
	void processInputKeys();

	void checkCursorInFrame();

	void checkBindedValues();
};

#endif //UI_TEXT_EDIT_H