#include "widget.h"

#include <algorithm>

#include "ui_manager.h"
#include "render/render.h"
#include "render/render_state/2d_render_state.h"
#include "ui_state.h"
#include "ui_property.h"
#include "ui_button.h"

REGIST_TYPE(uiWidget)

uiWidget::uiWidget( uiWidgetsManager* widgetsManager, const std::string& id /*= "noName"*/)
{
	mWidgetsManager = widgetsManager;
	mParent = NULL;
	mId = id;
	mTransparency = 0;
	mResTransparency = 0;
	mModal = false;

	mCurrentState = NULL;

	createStdStates();
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

	for (StatesList::const_iterator it = widget.mStates.cbegin(); it != widget.mStates.cend(); ++it)
		addState(new uiState(*(*it)));

	mCurrentState = NULL;
}

uiWidget::~uiWidget()
{
	removeAllChilds();
}

void uiWidget::addChild( uiWidget* widget )
{
	mChilds.push_back(widget);
	widget->mParent = this;

	if (getType() == uiWidget::getStaticType())
		adjustSizeByChilds();
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
	for (StatesList::iterator it = mStates.begin(); it != mStates.end(); ++it)
		(*it)->update(dt);

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

	derivedUpdate(dt);

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void uiWidget::draw()
{
	gr2DRenderStateBase* renderState = 
		static_cast<gr2DRenderStateBase*>(mWidgetsManager->mRender->getCurrentRenderState());

	/*color4 color(0.0f, 1.0f, 0.0f, mResTransparency);
	fRect rt(mGlobalPosition, mGlobalPosition + mSize);
	renderState->pushLine(rt.getltCorner(), rt.getrtCorner(), color);
	renderState->pushLine(rt.getrtCorner(), rt.getrdCorner(), color);
	renderState->pushLine(rt.getrdCorner(), rt.getldCorner(), color);
	renderState->pushLine(rt.getldCorner(), rt.getltCorner(), color);*/

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
		mPosition = vec2(0, 0);

	if (!serializeId(mOffset, "offset"))
		mOffset = vec2(0, 0);

	if (!serializeId(mModal, "modal"))
		mModal = false;

	if (!serializeId(mSize, "size"))
		mSize = vec2(0, 0);

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

	serializeObjPtrArrId(mStates, mStates.size(), "states");

	return true;
}

void uiWidget::addState( uiState* state )
{
	mStates.push_back(state);
	state->mTargetWidget = this;
}

void uiWidget::removeState( uiState* state )
{
	StatesList::iterator fnd = std::find(mStates.begin(), mStates.end(), state);

	if (fnd == mStates.end())
		return;

	if (mCurrentState == state)
	{
		state->deactivate(true);
		mCurrentState = NULL;
	}

	safe_release(state);

	mStates.erase(fnd);
}

void uiWidget::removeAllStates()
{
	for (StatesList::iterator it = mStates.begin(); it != mStates.end(); ++it)
		safe_release(*it);

	mCurrentState = NULL;

	mStates.clear();
}

uiState* uiWidget::getState( const std::string& id )
{
	for (StatesList::iterator it = mStates.begin(); it != mStates.end(); ++it)
		if ((*it)->mId == id) return *it;

	return NULL;
}

void uiWidget::setState( const std::string& id, bool forcible /*= false*/, bool recursive /*= false*/ )
{
	uiState* state = getState(id);

	if (!state && !recursive) 
	{
		//mWidgetsManager->mLog->fout(1, "WARNING: Can't find state '%s' in widget '%s'", id.c_str(), mId.c_str());
		//return;
	}

	if (state == mCurrentState && !recursive)
		return;

	if (mCurrentState && state)
		mCurrentState->deactivate(forcible);

	if (state) 
	{
		state->activate(forcible);
		mCurrentState = state;

		if (state->mId == "visible")
		{
			if (!mParent)
				mWidgetsManager->showedWidget(this);

			mVisible = true;
		}

		if (state->mId == "invisible")
		{
			if (!mParent)
				mWidgetsManager->hidedWidget(this);

			mVisible = false;
		}
	}

	if (recursive)
	{
		for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		{
			(*it)->setState(id, forcible, true);
		}
	}
}

void uiWidget::show( bool forcible /*= false*/ )
{
	setState("visible");
}

void uiWidget::hide( bool forcible /*= false*/ )
{
	setState("invisible");
}

void uiWidget::createStdStates()
{
	uiState* idleState = new uiState(this, "visible");
	idleState->addProperty(new uiParameterProperty<float>(&mTransparency, 1.0f));
	addState(idleState);

	uiState* hideState = new uiState(this, "invisible");
	hideState->addProperty(new uiParameterProperty<float>(&mTransparency, 0.0f));
	addState(hideState);

	show(true);
}

uiWidget* uiWidget::setPosition( const vec2& position )
{
	mPosition = position;
	return this;
}

vec2 uiWidget::getPosition() const
{
	return mPosition;
}

vec2 uiWidget::getGlobalPosition() const
{
	return mGlobalPosition;
}

uiWidget* uiWidget::setOffset( const vec2& offset )
{
	mOffset = offset;
	return this;
}

vec2 uiWidget::getOffset() const
{
	return mOffset;
}

uiWidget* uiWidget::setSize( const vec2& size )
{
	mSize = size;
	return this;
}

vec2 uiWidget::getSize() const
{
	return mSize;
}

bool uiWidget::isPointInside( const vec2& point )
{
	if (isPointInsideDerived(point))
		return true;

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		if ((*it)->isPointInside(point))
			return true;
	}

	return false;
}

bool uiWidget::isPointInsideDerived( const vec2& point )
{
	if (point.x < mGlobalPosition.x || point.x > mGlobalPosition.x + mSize.x ||
		point.y < mGlobalPosition.y || point.y > mGlobalPosition.y + mSize.y)
	{
		return false;
	}

	return true;
}

bool uiWidget::isModal() const
{
	return mModal;
}

uiWidget* uiWidget::setModal( bool modal )
{
	mModal = modal;
	return this;
}

int uiWidget::processInputMessageDerived( const cInputMessage& message )
{
	if (isPointInsideDerived(message.mCursorPosition))
		return 1;

	return 0;
}

int uiWidget::processInputMessage( const cInputMessage& message )
{
	if (getType() == uiButton::getStaticType())
	{
		volatile int i = 0;
		i++;
	}

	if (processInputMessageDerived(message) == 0) 
		return 0;

	int res = 0;
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); it++)
	{
		int widgetRes = (*it)->processInputMessage(message);
		if (widgetRes != 0)
			res = widgetRes;
	}

	return res;
}

void uiWidget::adjustSizeByChilds()
{
	vec2 vmin = mGlobalPosition, vmax = mGlobalPosition + mSize;

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		vec2 cmin = (*it)->mGlobalPosition;
		vec2 cmax = cmin + (*it)->mSize;

		if (cmin.x < vmin.x) vmin.x = cmin.x;
		if (cmin.y < vmin.y) vmin.y = cmin.y;
		if (cmax.x > vmax.x) vmax.x = cmax.x;
		if (cmax.y > vmax.y) vmax.y = cmax.y;
	}

	mPosition += vmin - mGlobalPosition;
	mSize = vmax - vmin;
}
