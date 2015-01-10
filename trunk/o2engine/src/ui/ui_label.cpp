#include "ui_label.h"

#include "render_system/text.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiLabel);

uiLabel::uiLabel(grFont* font, const layout& layout, const string& id /*= ""*/):
	uiWidget(layout, id), mText(NULL)
{
	mText = mnew grText(font);

	mResTransparencyChanged.add(callback<uiLabel>( this, &uiLabel::transparencyChanged));
}

uiLabel::uiLabel(const uiLabel& label):
	uiWidget(label)
{
	mText = mnew grText(*label.mText);

	mResTransparencyChanged.add(callback<uiLabel>( this, &uiLabel::transparencyChanged));
}

uiLabel::~uiLabel()
{
	safe_release(mText);
}

uiWidget* uiLabel::clone() const
{
	return mnew uiLabel(*this);
}

void uiLabel::setFont(grFont* font)
{
	mText->setFont(font);
}

grFont* uiLabel::getFont() const
{
	return mText->getFont();
}

void uiLabel::setText(const wstring& text)
{
	mText->setText(text);
}

wstring uiLabel::getText() const
{
	return mText->getText();
}

void uiLabel::setCText(const string& text)
{
	mText->setCText(text);
}

string uiLabel::getCText() const
{
	return mText->getCText();
}

vec2f uiLabel::getTextRealSize()
{
	return mText->getRealSize();
}

void uiLabel::setCharactersHeight(const float& height)
{
	mText->setCharactersHeight(height);
}

float uiLabel::getCharactersHeight() const
{
	return mText->getCharactersHeight();
}

void uiLabel::setHorAlign(const grFont::HorAlign& align)
{
	mText->setHorAlign(align);
}

grFont::HorAlign uiLabel::getHorAlign() const
{
	return mText->getHorAlign();
}

void uiLabel::setVerAlign(const grFont::VerAlign& align)
{
	mText->setVerAlign(align);
}

grFont::VerAlign uiLabel::getVerAlign() const
{
	return mText->getVerAlign();
}

void uiLabel::setWordWrap(const bool& flag)
{
	mText->setWordWrap(flag);
}

bool uiLabel::getWordWrap() const
{
	return mText->getWordWrap();
}

void uiLabel::setCharactersDistCoef(const float& coef /*= 1*/)
{
	mText->setCharactersDistCoef(coef);
}

float uiLabel::getCharactersDistCoef() const
{
	return mText->getCharactersDistCoef();
}

void uiLabel::setLinesDistCoef(const float& coef /*= 1*/)
{
	mText->setLinesDistCoef(coef);
}

float uiLabel::getLinesDistCoef() const
{
	return mText->getLinesDistCoef();
}

void uiLabel::localDraw()
{
	mText->draw();
}

void uiLabel::layoutUpdated()
{
	mText->setRect(mLayout.getRect());
}

void uiLabel::transparencyChanged()
{
	mText->setTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE