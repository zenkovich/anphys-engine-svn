#ifndef RECT_PACKER_H
#define RECT_PACKER_H

#include "public.h"
#include "util/containers/pool.h"
#include "util/containers/array.h"
#include "util/containers/tree_interface.h"

OPEN_O2_NAMESPACE

class RectsPacker
{
public:
	struct rect
	{
		int   mPage;
		fRect mRect;
		vec2f mSize;

		rect(const vec2f& size = vec2f()):mSize(size), mPage(-1) {}
	};

	typedef array< rect* > RectsArr;

protected:
	struct quadNode: public ITreeNode<quadNode>
	{
		fRect mRect;
		int   mPage;
		bool  mFree;

		quadNode(int page = 0, const fRect& rect = fRect()):mPage(page), mRect(rect), mFree(true) {}

		void onChildAdded(quadNode* child);

		bool operator==(const quadNode& other);
	};
	typedef array<quadNode> NodesArr;

	pool<rect> mRectsPool;
	RectsArr   mRects;
	NodesArr   mQuadNodes;
	vec2f      mMaxSize;

public:
	RectsPacker(const vec2f&  maxSize = vec2f(512, 512));
	~RectsPacker();

	rect* addRect(const vec2f&  size);
	void removeRect(rect* remRect);
	void clear();

	void setMaxSize(const vec2f&  maxSize);
	vec2f getMaxSize() const;

	bool pack();

protected:
	bool insertRect(rect& rt);
	bool tryInsertRect(rect& rt, quadNode& node);
	void createNewPage();

	static bool rectSizeCompare(rect*& a, rect*& b);
};

CLOSE_O2_NAMESPACE

#endif //RECT_PACKER_H