#include "sprite_widget.h"

#include "render/render_objects/2d/sprite.h"
#include "util/serialization/data_objects_manager.h"
#include "ui_manager.h"

REGIST_TYPE(uiSpriteWidget)

uiSpriteWidget::uiSpriteWidget(uiWidgetsManager* widgetsManager, grSprite* sprite /*= NULL*/, bool ownSprite /*= true*/, 
	                           const std::string& id /*= "noName"*/):
	uiWidget(widgetsManager, id), mSprite(sprite), mOwnSprite(ownSprite)
{
	mSize = mSprite->getSize().scale(mSprite->getScale());
}

uiSpriteWidget::uiSpriteWidget(uiWidgetsManager* widgetsManager, 
	                           const std::string& spriteSrcFile, const std::string& spriteSrcObjectpath,
	                           const std::string& id /*= "noName"*/):
	uiWidget(widgetsManager, id), mSprite(NULL), mOwnSprite(false), mSpriteSrcFile(spriteSrcFile), 
	mSpriteSrcObjectPath(mSpriteSrcObjectPath)
{
	mSprite = new grSprite(widgetsManager->mRender, spriteSrcFile, spriteSrcObjectpath);
	mSize = mSprite->getSize().scale(mSprite->getScale());
	mOwnSprite = true;
}

uiSpriteWidget::uiSpriteWidget( uiWidgetsManager* widgetsManager, cDataObject* dataObject):
	uiWidget(widgetsManager, "noname")
{
	serialize(*dataObject, AT_INPUT, "");
}

uiSpriteWidget::uiSpriteWidget(const uiSpriteWidget& spriteWidget):
	uiWidget(spriteWidget)
{
	mOwnSprite = spriteWidget.mOwnSprite;

	if (spriteWidget.mOwnSprite)
	{
		mSprite = spriteWidget.mSprite->clone();
	}
	else
	{
		mSprite = spriteWidget.mSprite;
	}
}

uiSpriteWidget::~uiSpriteWidget()
{
	if (mOwnSprite) 
		safe_release(mSprite);
}

void uiSpriteWidget::derivedUpdate( float dt )
{
	if (mSprite)
	{
		color4 spriteColor = mSprite->getColor();
		mSprite->setPosition(mGlobalPosition).
			     setColor(color4(spriteColor.r, spriteColor.g, spriteColor.b, (int)(255.0f*mResTransparency)));
	}
}

void uiSpriteWidget::draw()
{
	if (mOwnSprite) 
		mSprite->draw();

	uiWidget::draw();
}

uiWidget* uiSpriteWidget::clone() const
{
	return new uiSpriteWidget(*this);
}

serializeMethodImpl(uiSpriteWidget)
{
	if (achieveType == AT_OUTPUT)
	{
		if (!mOwnSprite) return true;

		if (mSpriteSrcFile != "")
		{
			serializeId(mSpriteSrcFile, "spriteSrcFile");
			serializeId(mSpriteSrcObjectPath, "spriteSrcObjectpath");
		}
		else
		{
			serializeObjId(*mSprite, "sprite");	
		}
	}
	else //AT_INPUT
	{
		if (mOwnSprite) 
			safe_release(mSprite);

		if (serializeId(mSpriteSrcFile, "spriteSrcFile"))
		{
			serializeId(mSpriteSrcObjectPath, "spriteSrcObjectpath");

			mSprite = new grSprite(mWidgetsManager->mRender, mSpriteSrcFile, mSpriteSrcObjectPath);
			mOwnSprite = true;
		}
		else
		{
			cDataObject* spriteDataObject = dataArchieve.getChild("sprite");
			if (spriteDataObject)
			{
				mSprite = new grSprite(mWidgetsManager->mRender, *spriteDataObject);
				mOwnSprite = true;
			}
			else
			{
				mOwnSprite = false;
			}
		}
	}

	return true;
}

uiSpriteWidget* uiSpriteWidget::setSize( const vec2& size )
{
	mSize = size;
	mSprite->setSize(size);
	return this;
}
