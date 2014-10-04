#ifndef TREE_INTERFACE_H
#define TREE_INTERFACE_H

#include "public.h"
#include "array.h"
#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

template<typename _type>
class ITreeNode
{
public:
	typedef array<_type*> ChildArr;

protected:
	_type*   _this;
	_type*   mParent;
	ChildArr mChilds;

public:
	ITreeNode();

	virtual ~ITreeNode();

	virtual _type* addChild(_type* node);

	virtual bool removeChild(_type* node, bool release = true);

	virtual void removeAllChilds();

	virtual void setParent(_type* parent);

	virtual _type* getParent() const;

	virtual ChildArr& getChilds()
	{
		return mChilds;
	}
	
	virtual void onChildAdded(_type* child) {}

	virtual void onChildRemoved(_type* child) {}
};

//implementation
template<typename _type>
ITreeNode<_type>::ITreeNode():
	mParent(0)
{
	_this = dynamic_cast<_type*>(this);
}

template<typename _type>
ITreeNode<_type>::~ITreeNode()
{
	removeAllChilds();
}

template<typename _type>
_type* ITreeNode<_type>::addChild(_type* node)
{
	if (node->getParent())
		node->getParent()->removeChild(node, false);

	node->mParent = _this;

	mChilds.add(node);

	onChildAdded(node);

	return node;
}

template<typename _type>
bool ITreeNode<_type>::removeChild(_type* node, bool release = true)
{
	node->mParent = NULL;

	if (!mChilds.remove(node))
		return false;

	onChildRemoved(node);

	if (release)
		safe_release(node);

	return true;
}

template<typename _type>
void ITreeNode<_type>::removeAllChilds()
{
	foreach(ChildArr, mChilds, child)
		onChildRemoved(*child);

	foreach(ChildArr, mChilds, child)
		safe_release(*child);

	mChilds.clear();
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
_type* ITreeNode<_type>::getParent() const
{
	return mParent;
}

CLOSE_O2_NAMESPACE

#endif // TREE_INTERFACE_H
