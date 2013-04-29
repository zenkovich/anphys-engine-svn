#include "ui_label.h"

#include "ui_font.h"
#include "ui_manager.h"

REGIST_TYPE(uiLabel)

uiLabel::uiLabel( uiWidgetsManager* widgetsManager, uiFont* font, const std::string& id ):
	uiWidget(widgetsManager, id), mFont(font)
{
}

uiLabel::uiLabel( uiWidgetsManager* widgetsManager, const std::string& fontFile, const std::string& fontFilePath,
	              const std::string& id):
	uiWidget(widgetsManager, id)
{
	mFont = new uiFont(widgetsManager->mRender);
	mFont->load(fontFile,  fontFilePath);
}

uiLabel::uiLabel( const uiLabel& label ):
	uiWidget(label)
{
	mFont = label.mFont->clone();
}

uiLabel::~uiLabel()
{
	safe_release(mFont);
}

uiLabel* uiLabel::setText( const std::string& text )
{
	mFont->setText(text);
	return this;
}

std::string& uiLabel::getText()
{
	return mFont->getText();
}

uiLabel* uiLabel::setHorAlign( HorAlign align )
{
	mFont->setHorAlign((uiFont::HorAlign)( (unsigned int)align ));
	return this;
}

uiLabel::HorAlign uiLabel::getHorAlign() const
{
	return (HorAlign)( (unsigned int)mFont->getHorAlign() );
}

uiLabel* uiLabel::setVerAlign( VerAlign align )
{
	mFont->setVerAlign((uiFont::VerAlign)( (unsigned int)align ));
	return this;
}

uiLabel::VerAlign uiLabel::getVerAlign() const
{
	return (VerAlign)( (unsigned int)mFont->getVerAlign() );
}

void uiLabel::derivedUpdate( float dt )
{
	fRect currentRect(mGlobalPosition, mGlobalPosition + mSize);
	if (currentRect != mFont->getTextArea())
		mFont->setTextArea(currentRect);

	color4 fontColor = mFont->getColor();
	if (fabs(fontColor.af() - mResTransparency) > FLT_EPSILON)
	{
		fontColor.a = (int)(255.0f*mResTransparency);
		mFont->setColor(fontColor);
	}
}

void uiLabel::derivedDraw()
{
	mFont->draw();
}

uiLabel* uiLabel::clone() const
{
	return new uiLabel(*this);
}
