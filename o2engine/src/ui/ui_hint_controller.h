#ifndef UI_HINT_CONTROLLER_H
#define UI_HINT_CONTROLLER_H

#include "public.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiLabel;

/** Hint controller. Processing hint widget. */
class uiHintController
{
	uiWidget* mWidget;            /** Main hint widget. */
	uiLabel*  mLabel;             /** Label in hint widget. */

	string    mNextPlaceHintText; /** Text for next showing hint. */
	vec2f     mNextPlacePosition; /** Position for placing hint. */
	bool      mNeedShow;

public:
	/** ctor. */
	uiHintController();

	/** dtor. */
	~uiHintController();

	void setupWidget(uiWidget* widget, uiLabel* label);

	void update(float dt);
	void draw();

	void showAt(const vec2f& position, const string& hintString);
	void hide();

private:
	void widgetDisappeared();
	void beginAppearing();
};

CLOSE_O2_NAMESPACE

#endif //UI_HINT_CONTROLLER_H
