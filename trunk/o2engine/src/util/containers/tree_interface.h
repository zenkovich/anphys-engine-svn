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
	ITreeNode();

	virtual ~ITreeNode();

	virtual _type* addChild(_type* node);

	virtual bool removeChild(_type* node, bool release = true);

	virtual void removeAllChilds();

	virtual void setParent(_type* parent);

	virtual _type* getParent() const;
};

CLOSE_O2_NAMESPACE

#endif // TREE_INTERFACE_H
