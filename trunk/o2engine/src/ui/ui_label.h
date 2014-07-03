#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class grText;

class uiLabel: public uiWidget
{
	grText* mText;
};

CLOSE_O2_NAMESPACE

#endif // UI_LABEL_H
