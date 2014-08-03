#ifndef UI_HINT_CONTROLLER_H
#define UI_HINT_CONTROLLER_H

#include "public.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiLabel;

class uiHintController
{
	uiWidget* mWidget;
	uiLabel*  mLabel;

	string    mNextPlaceHintText;
	vec2f     mNextPlacePosition;
	bool      mNeedShow;

public:
	uiHintController();
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
