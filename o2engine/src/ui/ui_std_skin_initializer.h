#ifndef UI_STD_SKIN_INITIALIZER_H
#define UI_STD_SKIN_INITIALIZER_H

#include "public.h"

#include "ui_skin.h"

OPEN_O2_NAMESPACE

class grFont;

class uiStdSkinInitializer
{
	static uiSkinManager* mSkinManager;
	static grFont*        mStdFont;

public:
	static void initialize();
	static void deinitialize();

protected:
	static void initButton();
	static void initBackground();
	static void initProgressBar();
	static void initCheckBox();
	static void initHorScrollBar();
	static void initEditBox();
	static void initLabel();
};

CLOSE_O2_NAMESPACE

#endif //UI_STD_SKIN_INITIALIZER_H
