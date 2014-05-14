#include "node_component.h"

#include "scene_node.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(scComponent);


scComponent::scComponent(const shared(scNode)& ownerNode):
	mOwnerNode(ownerNode)
{
}

scComponent::~scComponent()
{
}

void scComponent::setEnable(const bool& enable)
{
	mEnable = enable;

	if (mEnable) 
	{
		if (mOnEnabled) 
			mOnEnabled->call();
	}
	else
	{
		if (mOnDisabled)
			mOnDisabled->call();
	}
}

bool scComponent::isEnable() const
{
	return mEnable;
}

shared(scNode) scComponent::node() const
{
	return mOwnerNode;
}

void scComponent::setOnEnabled(const shared(ICallback)& callback)
{
	mOnEnabled = callback;
}

shared(ICallback) scComponent::getOnEnabled() const
{
	return mOnEnabled;
}

void scComponent::setOnDisabled(const shared(ICallback)& callback)
{
	mOnDisabled = callback;
}

shared(ICallback) scComponent::getOnDisabled() const
{
	return mOnDisabled;
}

void scComponent::initializeProperties()
{
	enable.init(this, &scComponent::setEnable, &scComponent::isEnable);
	onEnabled.init(this, &scComponent::setOnEnabled, &scComponent::getOnEnabled);
	onEnabled.init(this, &scComponent::setOnDisabled, &scComponent::getOnDisabled);
}

CLOSE_O2_NAMESPACE