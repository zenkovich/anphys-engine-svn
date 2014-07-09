#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "ui_widget.h"
#include "render_system/font.h"

OPEN_O2_NAMESPACE

class grText;

class uiLabel: public uiWidget
{
	grText* mText; /** Basic text drawable. */

public:
	DEFINE_TYPE(uiLabel);

	//properties
	PROPERTY(uiLabel, grFont*)          font;                /** Font pointer property. Uses set/getFont. */
	PROPERTY(uiLabel, wstring)          text;                /** Text property, wstring. Uses set/getText. */
	PROPERTY(uiLabel, string)           ctext;               /** Text property, string. Uses set/getCText. */
	PROPERTY(uiLabel, grFont::VerAlign) verAlign;            /** vertical align property. Uses set/getVerAlign. */
	PROPERTY(uiLabel, grFont::HorAlign) horAlign;            /** Horizontal align property. Uses set/getHorAlign. */
	PROPERTY(uiLabel, bool)             wordWrap;            /** Words wrapping flag property. Uses set/getWordWrap. */
	PROPERTY(uiLabel, float)            charactersHeight;    /** Characters height property, pixels. Uses set/getCharactersHeight. */
	PROPERTY(uiLabel, float)            charactersDistCoef;  /** Characters distance coef, 1 is standard. Uses set/getCharactersDistCoef. */
	PROPERTY(uiLabel, float)            linesDistCoef;       /** Lines distance coef, 1 is standard. Uses set/getLinesDistCoef. */
	
	/** ctor. */
	uiLabel(grFont* font, const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiLabel(const uiLabel& label);

	/** dtor. */
	~uiLabel();

	/** Returns copy of widget. */
	uiWidget* clone() const;		

	/** Sets using font. */
	void setFont(grFont* font);

	/** Returns using font. */
	grFont* getFont() const;

	/** Sets text. */
	void setText(const wstring& text);

	/** Returns text. */
	wstring getText() const;

	/** Sets text. */
	void setCText(const string& text);

	/** Returns text as string. */
	string getCText() const;

	/** Sets characters height in pixels. */
	void setCharactersHeight(const float& height);

	/** Returns characters height in pixels. */
	float getCharactersHeight() const;

	/** Sets horizontal align. */
	void setHorAlign(const grFont::HorAlign& align);

	/** Returns horizontal align. */
	grFont::HorAlign getHorAlign() const;

	/** Sets vertical align. */
	void setVerAlign(const grFont::VerAlign& align);

	/** returns vertical align. */
	grFont::VerAlign getVerAlign() const;

	/** Sets word wrapping. */
	void setWordWrap(const bool& flag);

	/** Returns word wrapping. */
	bool getWordWrap() const;

	/** Sets characters distance coefficient. */
	void setCharactersDistCoef(const float& coef = 1);

	/** Returns characters distance coef. */
	float getCharactersDistCoef() const;

	/** Sets lines distance coefficient. */
	void setLinesDistCoef(const float& coef = 1);

	/** Returns lines distance coefficient. */
	float getLinesDistCoef() const;

protected:
	/** Drawing current widget. */
	virtual void localDraw();

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();
};

CLOSE_O2_NAMESPACE

#endif // UI_LABEL_H
