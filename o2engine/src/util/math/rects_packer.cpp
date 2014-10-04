#include "rects_packer.h"

#include "util/containers/utils.h"

OPEN_O2_NAMESPACE

cRectsPacker::cRectsPacker(const vec2f& maxSize):mMaxSize(maxSize), mRectsPool(25, 25)
{
}

cRectsPacker::~cRectsPacker()
{
	clear();
}

cRectsPacker::rect* cRectsPacker::addRect(const vec2f& size)
{
	rect* newRect = mRectsPool.take();
	newRect->mSize = size;
	newRect->mRect = fRect();
	mRects.add(newRect);
	return newRect;
}

void cRectsPacker::removeRect(rect* remRect)
{
	mRects.remove(remRect);
	mRectsPool.free(remRect);
}

void cRectsPacker::clear()
{
	foreach(RectsArr, mRects, rt)
		mRectsPool.free(*rt);

	mRects.clear();
}

void cRectsPacker::setMaxSize(const vec2f& maxSize)
{
	mMaxSize = maxSize;
}

vec2f cRectsPacker::getMaxSize() const
{
	return mMaxSize;
}

bool cRectsPacker::pack()
{
	mQuadNodes.clear();

	mRects.sort(&rectSizeCompare);

	foreach(RectsArr, mRects, rt)
		if (!insertRect(**rt))
			return false;

	return true;
}

bool cRectsPacker::rectSizeCompare( rect*& a, rect*& b )
{
	return a->mSize.x*a->mSize.y > b->mSize.x*b->mSize.y;
}

void cRectsPacker::createNewPage()
{
	int maxPage = -1;
	foreach(RectsArr, mRects, rt)
		maxPage = max(maxPage, (*rt)->mPage);

	mQuadNodes.add( quadNode(maxPage + 1, fRect(vec2f(), mMaxSize)) );
}

bool cRectsPacker::insertRect(rect& rt)
{
	foreach(NodesArr, mQuadNodes, node)
		if (tryInsertRect(rt, *node))
			return true;

	createNewPage();
	
	foreach(NodesArr, mQuadNodes, node)
		if (tryInsertRect(rt, *node))
			return true;

	return false;
}

bool cRectsPacker::tryInsertRect(rect& rt, quadNode& node)
{
	if (node.mFree && node.mRect.getSizeX() >= rt.mSize.x && 
		              node.mRect.getSizeY() >= rt.mSize.y)
	{
		vec2f leftTop = node.mRect.getltCorner();
		vec2f center = leftTop + rt.mSize;
		vec2f rightDown = node.mRect.getrdCorner();

		node.mFree = false;
		node.addChild(mnew quadNode(node.mPage, fRect(center.x, leftTop.y, rightDown.x, center.y)));
		node.addChild(mnew quadNode(node.mPage, fRect(center.x, center.y,  rightDown.x, rightDown.y)));
		node.addChild(mnew quadNode(node.mPage, fRect(leftTop.x, center.y,  center.x, rightDown.y)));

		rt.mPage = node.mPage;
		rt.mRect = fRect(leftTop, center);

		return true;
	}

	foreach(quadNode::ChildArr, node.getChilds(), childNode)
		if (tryInsertRect(rt, **childNode))
			return true;

	return false;
}

CLOSE_O2_NAMESPACE