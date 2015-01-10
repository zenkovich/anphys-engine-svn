#include "ui_context_menu.h"

#include "ui_button.h"
#include "ui_label.h"
#include "ui_sprite.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiContextMenu);


uiContextMenu::uiContextMenu(const layout& layout, const string& id /*= ""*/):
	uiDrawablesListWidget(layout, id)
{

}

uiContextMenu::uiContextMenu(const uiContextMenu& contextMenu):
	uiDrawablesListWidget(contextMenu)
{
}

uiContextMenu::~uiContextMenu()
{
	safe_release(mButtonSample);
	safe_release(mDelimerSample);
}

uiWidget* uiContextMenu::clone() const
{
	return mnew uiContextMenu(*this);
}

uiButton* uiContextMenu::addButtonElement(const wstring& caption, const string& key /*= ""*/, 
	                                      const string& iconTextureName /*= ""*/, int position /*= -1*/)
{
	uiButton* newButton = mnew uiButton(*mButtonSample);
	newButton->setCaption(caption);

	uiLabel* keyLabel = newButton->getWidgetByType<uiLabel>("hintKeyLabel");
	if (keyLabel)
		keyLabel->setCText(key);

	uiSprite* iconSprite = newButton->getWidgetByType<uiSprite>("iconSprite");
	if (iconSprite)
	{
		if (iconTextureName.empty())
			iconSprite->setVisible(false);
		else
			iconSprite->mSprite.setTexture(grTexture::createFromFile(iconTextureName));
	}

	addChild(newButton, position);

	return newButton;
}

uiWidget* uiContextMenu::addDelimer(int position /*= -1*/)
{
	uiWidget* newDelimer = mDelimerSample->clone();

	addChild(newDelimer, position);

	return newDelimer;
}

uiWidget* uiContextMenu::addElement(uiWidget* widget, int position /*= -1*/)
{
	addChild(widget, position);
	return widget;
}

void uiContextMenu::setupElementsSamples(uiWidget* delimer, uiButton* button)
{
	mDelimerSample = delimer->clone();
	mButtonSample = mnew uiButton(*button);
}

void uiContextMenu::layoutUpdated()
{

}

bool uiContextMenu::localProcessInputMessage(const InputMessage& msg)
{
	return false;
}

CLOSE_O2_NAMESPACE