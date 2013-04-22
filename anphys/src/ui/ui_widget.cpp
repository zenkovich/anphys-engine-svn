#include "ui_widget.h"

#include <algorithm>

#include "render/render.h"
#include "render/render_state/2d_render_state.h"
#include "render/render_target/stencil_buffer_render_target.h"
#include "ui_button.h"
#include "ui_manager.h"
#include "ui_property.h"
#include "ui_state.h"

REGIST_TYPE(uiWidget)

uiWidget::uiWidget( uiWidgetsManager* widgetsManager, const std::string& id /*= "noName"*/)
{
	mWidgetsManager = widgetsManager;
	mParent = NULL;
	mId = id;
	mTransparency = 0;
	mResTransparency = 0;
	mModal = false;
	mIsClipping = false;
	mClippingStencilBuffer = NULL;

	createStdStates();

	update(0.0f);
}

uiWidget::uiWidget( const uiWidget& widget )
{
	mWidgetsManager = widget.mWidgetsManager;
	mId = widget.mId;
	mParent = NULL;
	mTransparency = widget.mTransparency;
	mResTransparency = widget.mResTransparency;
	mModal = widget.mModal;
	mIsClipping = false;
	setClipping(widget.isClipping());
	mClippingStencilBuffer = NULL;

	for (WidgetsList::const_iterator it = widget.mChilds.cbegin(); it != widget.mChilds.cend(); ++it)
		addChild((*it)->clone());

	mVisibleState = new uiState(*widget.mVisibleState);

	update(0.0f);
}

uiWidget::~uiWidget()
{
	safe_release(mVisibleState);
	removeAllChilds();
}

void uiWidget::addChild( uiWidget* widget )
{
	mChilds.push_back(widget);
	widget->mParent = this;

	if (getType() == uiWidget::getStaticType())
		adjustSizeByChilds();

	update(0.0f);
}

void uiWidget::removeChild( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mChilds.begin(), mChilds.end(), widget);

	if (fnd == mChilds.end())
		return;

	mChilds.erase(fnd);

	safe_release(widget);

	if (getType() == uiWidget::getStaticType())
		adjustSizeByChilds();

	update(0.0f);
}

void uiWidget::removeAllChilds()
{
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		safe_release(*it);

	mChilds.clear();

	if (getType() == uiWidget::getStaticType())
		adjustSizeByChilds();

	update(0.0f);
}

uiWidget* uiWidget::getWidget( const std::string& id ) const
{
	int slashPos = id.find('/');

	std::string searchId;
	if (slashPos == id.npos) searchId = id;
	else                     searchId = id.substr(0, slashPos);

	if (searchId == "..")
	{
		if (mParent)
			return mParent->getWidget(id.substr(slashPos + 1));
		else
			return NULL;
	}

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
	setupInitialProperties();
	for (PropertyList::iterator it = mPropertyList.begin(); it != mPropertyList.end(); ++it)
	{
		(*it)->update(dt);
	}

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
	if (mIsClipping)
	{
		mWidgetsManager->mRender->bindRenderTarget(mClippingStencilBuffer);
		mClippingStencilBuffer->clear();
		mClippingStencilBuffer->fillRect(fRect(mGlobalPosition - vec2(1.0f, 1.0f), mGlobalPosition + mResSize));
		mWidgetsManager->mRender->unbindRenderTarget(mClippingStencilBuffer);

		mWidgetsManager->mRender->bindStencilBuffer(mClippingStencilBuffer);
	}

	derivedDraw();

	gr2DRenderStateBase* renderState = 
		static_cast<gr2DRenderStateBase*>(mWidgetsManager->mRender->getCurrentRenderState());

	if (getType() == uiWidget::getStaticType() || true)
	{
		color4 color(0.4f, 0.4f, 0.4f, mResTransparency);
		fRect rt(mGlobalPosition, mGlobalPosition + mResSize);
		renderState->pushLine(rt.getltCorner(), rt.getrtCorner(), color);
		renderState->pushLine(rt.getrtCorner(), rt.getrdCorner(), color);
		renderState->pushLine(rt.getrdCorner(), rt.getldCorner(), color);
		renderState->pushLine(rt.getldCorner(), rt.getltCorner(), color);
		renderState->flush();
	}

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->draw();
	}

	if (mClippingStencilBuffer)
	{
		mWidgetsManager->mRender->unbindStencilBuffer();
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

	return true;
}

void uiWidget::show( bool forcible /*= false*/ )
{
	mVisibleState->activate(forcible);
	mVisible = true;
	mWidgetsManager->showedWidget(this);
}

void uiWidget::hide( bool forcible /*= false*/ )
{
	mVisibleState->deactivate(forcible);
	mVisible = false;
	mWidgetsManager->hidedWidget(this);
}
 
void uiWidget::createStdStates()
{
	mVisibleState = new uiState(this, "visible");
	mVisibleState->addProperty(
		new uiParameterProperty<float>(&mTransparency, 0.0f, 1.0f, uiProperty::IT_SMOOTH, 0.15f, 
		uiParameterProperty<float>::OP_MULTIPLICATION));

	setupInitialProperties();
	show(true);
}

uiWidget* uiWidget::setPosition( const vec2& position )
{
	mPosition = position;
	update(0.0f);
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
	update(0.0f);
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
	if (point.x < mGlobalPosition.x || point.x > mGlobalPosition.x + mResSize.x ||
		point.y < mGlobalPosition.y || point.y > mGlobalPosition.y + mResSize.y)
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
	vec2 vmin = mGlobalPosition, vmax = mGlobalPosition + mResSize;

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		vec2 cmin = (*it)->mGlobalPosition;
		vec2 cmax = cmin + (*it)->mResSize;

		if (cmin.x < vmin.x) vmin.x = cmin.x;
		if (cmin.y < vmin.y) vmin.y = cmin.y;
		if (cmax.x > vmax.x) vmax.x = cmax.x;
		if (cmax.y > vmax.y) vmax.y = cmax.y;
	}

	mPosition += vmin - mGlobalPosition;
	mSize = vmax - vmin;
}

bool compareProperties( uiProperty* a, uiProperty* b )
{
	return a->mPriority < b->mPriority;
}

void uiWidget::registProperty( uiProperty* property )
{
	mPropertyList.push_back(property);
	std::sort(mPropertyList.begin(), mPropertyList.end(), compareProperties);
}

void uiWidget::unregistProperty( uiProperty* property )
{
	PropertyList::iterator fnd = std::find(mPropertyList.begin(), mPropertyList.end(), property);
	if (fnd != mPropertyList.end())
		mPropertyList.erase(fnd);
}

void uiWidget::setupInitialProperties()
{
	mTransparency = 1.0f;
	mOffset = vec2(0, 0);
	mResSize = mSize;
}

uiWidget* uiWidget::setClipping( bool flag )
{
	if (flag == mIsClipping)
		return this;

	mIsClipping = flag;
	if (flag && !mClippingStencilBuffer)
	{
		mClippingStencilBuffer = new grStencilBufferRenderTarget(mWidgetsManager->mRender);
	}

	return this;
}

bool uiWidget::isClipping() const
{
	return mIsClipping;
}
