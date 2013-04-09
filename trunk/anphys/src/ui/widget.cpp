#include "widget.h"

#include <algorithm>

#include "ui_manager.h"

REGIST_TYPE(uiWidget)

uiWidget::uiWidget( uiWidgetsManager* widgetsManager, const std::string& id /*= "noName"*/, uiWidget* parent /*= NULL*/ )
{
	mWidgetsManager = widgetsManager;
	mId = id;
	if (parent) parent->addChild(this);
	mTransparency = 0;
	mResTransparency = 0;
	mModal = false;
}

uiWidget::uiWidget( const uiWidget& widget )
{
	mWidgetsManager = widget.mWidgetsManager;
	mId = widget.mId;
	mParent = NULL;
	mTransparency = widget.mTransparency;
	mResTransparency = widget.mResTransparency;
	mModal = widget.mModal;

	for (WidgetsList::const_iterator it = widget.mChilds.cbegin(); it != widget.mChilds.cend(); ++it)
		addChild((*it)->clone());
}

uiWidget::~uiWidget()
{
	removeAllChilds();
}

void uiWidget::addChild( uiWidget* widget )
{
	mChilds.push_back(widget);
	widget->mParent = this;
}

void uiWidget::removeChild( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mChilds.begin(), mChilds.end(), widget);

	if (fnd == mChilds.end())
		return;

	mChilds.erase(fnd);

	safe_release(widget);
}

void uiWidget::removeAllChilds()
{
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		safe_release(*it);

	mChilds.clear();
}

uiWidget* uiWidget::getWidget( const std::string& id ) const
{
	int slashPos = id.find('/');

	std::string searchId;
	if (slashPos == id.npos) searchId = id;
	else                     searchId = id.substr(0, slashPos);

	for (WidgetsList::const_iterator it = mChilds.cbegin(); it != mChilds.cend(); ++it)
	{
		if ((*it)->mId != searchId) continue;

		if (slashPos == id.npos) 
			return *it;
		else
			return (*it)->getWidget(id.substr(slashPos + 1));
	}

	return NULL;
}

void uiWidget::update( float dt )
{
	if (mParent) 
	{
		mResTransparency = mParent->mResTransparency*mTransparency;
		mGlobalPosition = mParent->mGlobalPosition + mPosition + mOffset;
	}
	else
	{
		mResTransparency = mTransparency;
		mGlobalPosition = mPosition + mOffset;
	}

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void uiWidget::draw()
{
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->draw();
	}
}

uiWidget* uiWidget::clone() const
{
	return new uiWidget(*this);
}

serializeMethodImpl(uiWidget)
{
	serializeId(mId, "id");

	if (!serializeId(mPosition, "position"))
		mPosition = vec2(0);

	if (!serializeId(mOffset, "offset"))
		mOffset = vec2(0);

	if (!serializeId(mModal, "modal"))
		mModal = false;

	if (achieveType == AT_OUTPUT)
	{
		cDataObject* childsWidgetsObject = dataArchieve.addChild("childs");

		for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		{
			cDataObject* childObject = childsWidgetsObject->addChild("child");

			childObject->addChild((*it)->getTypeName(), "widgetType");

			(*it)->serialize(*childObject, AT_OUTPUT, "");
		}
	}
	else //AT_INPUT
	{
		cDataObject* childObject = dataArchieve.getChild("childs");
		if (childObject)
		{
			for (cDataObject::DataObjectsList::iterator it = childObject->mChilds.begin();
				 it != childObject->mChilds.end(); ++it)
			{
				if ((*it)->mId != "child") 
					continue;

				uiWidget* newChild = mWidgetsManager->createWidget((*it));
				addChild(newChild);
			}
		}
	}

	return true;
}
