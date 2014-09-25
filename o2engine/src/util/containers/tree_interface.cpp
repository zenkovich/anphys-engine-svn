#include "tree_interface.h"

OPEN_O2_NAMESPACE
	
template<typename _type>
_type* ITreeNode<_type>::getParent() const
{
	return mParent;
}

template<typename _type>
void ITreeNode<_type>::setParent(_type* parent)
{
	if (parent)
		parent->addChild(_this);
	else
	{
		if (mParent)
			mParent->removeChild(_this, false);

		mParent = NULL;
	}
}

template<typename _type>
void ITreeNode<_type>::removeAllChilds()
{
	foreach(_type*, mChilds, child)
		safe_release(*child);

	mChilds.clear();
}

template<typename _type>
bool ITreeNode<_type>::removeChild(_type* node, bool release /*= true*/)
{
	node->mParent = NULL;

	if (!mChilds.remove(node))
		return false;

	if (release)
		safe_release(node);

	return true;
}

template<typename _type>
_type* ITreeNode<_type>::addChild(_type* node)
{
	if (node->getParent())
		node->getParent()->removeChild(node, false);

	node->mParent = _this;

	mChilds.add(node);
	return node;
}

template<typename _type>
ITreeNode<_type>::~ITreeNode()
{
	removeAllChilds();
}

template<typename _type>
ITreeNode<_type>::ITreeNode() :mParent(0)
{
	_this = dynamic_cast<_type*>(this);
}

CLOSE_O2_NAMESPACE