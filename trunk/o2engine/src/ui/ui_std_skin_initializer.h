#ifndef UI_STD_SKIN_INITIALIZER_H
#define UI_STD_SKIN_INITIALIZER_H

#include "public.h"

#include "ui_skin.h"

OPEN_O2_NAMESPACE

class uiStdSkinInitializer
{
	static shared<uiSkinManager> mSkinManager;

public:
	static void initialize();
	static void deinitialize();

protected:
	static void initButton();
	static void initBackground();
};

CLOSE_O2_NAMESPACE

#endif //UI_STD_SKIN_INITIALIZER_H
