#include "node_component.h"

#include "scene_node.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(scComponent);


scComponent::scComponent(scNode* ownerNode):
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
		onEnalbedEvent.call();
	else
		onDisabledEvent.call();
}

bool scComponent::isEnable() const
{
	return mEnable;
}

scNode* scComponent::node() const
{
	return mOwnerNode;
}

void scComponent::initializeProperties()
{
	REG_PROPERTY(scComponent, enable, setEnable, isEnable);
}

CLOSE_O2_NAMESPACE