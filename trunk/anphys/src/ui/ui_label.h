#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "ui_widget.h"

struct uiFont;

struct uiLabel:public uiWidget
{
	DEFINE_TYPE(uiLabel)

	enum HorAlign { AL_LEFT = 0, AL_CENTER, AL_RIGHT };
	enum VerAlign { AL_TOP = 0, AL_MIDDLE, AL_BOTTOM };

	uiFont* mFont;

///functions
	uiLabel(uiWidgetsManager* widgetsManager, uiFont* font, const std::string& id);
	uiLabel(uiWidgetsManager* widgetsManager, const std::string& fontFile, const std::string& fontFilePath, 
		    const std::string& id);
	uiLabel(const uiLabel& label);
	~uiLabel();

	uiLabel*     setText(const std::string& text);
	std::string& getText();

	uiLabel*     setHorAlign(HorAlign align);
	HorAlign     getHorAlign() const;

	uiLabel*     setVerAlign(VerAlign align);
	VerAlign     getVerAlign() const;

	void         derivedUpdate(float dt);
	void         derivedDraw();

	uiLabel*     clone() const;
};

#endif //UI_LABEL_H