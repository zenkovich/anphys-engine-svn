#ifndef TREE_INTERFACE_H
#define TREE_INTERFACE_H

#include "public.h"
#include "array.h"
#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

template<typename _type>
class ITreeNode
{
protected:
	_type*        _this;
	_type*        mParent;
	array<_type*> mChilds;

public:
	ITreeNode():mParent(0)
	{
		_this = dynamic_cast<_type*>(this);
	}

	virtual ~ITreeNode()
	{
		removeAllChilds();
	}

	virtual _type* addChild(_type* node)
	{
		if (node->getParent())
			node->getParent()->removeChild(node, false);

		node->mParent = _this;

		mChilds.add(node);
		return node;
	}

	virtual bool removeChild(_type* node, bool release = true)
	{
		node->mParent = NULL;

		if (!mChilds.remove(node))
			return false;

		if (release)
			safe_release(node);

		return true;
	}

	virtual void removeAllChilds()
	{
		foreach(_type*, mChilds, child)
			safe_release(*child);

		mChilds.clear();
	}

	virtual void setParent(_type* parent)
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

	virtual _type* getParent() const
	{
		return mParent;
	}
};

CLOSE_O2_NAMESPACE

#endif // TREE_INTERFACE_H
