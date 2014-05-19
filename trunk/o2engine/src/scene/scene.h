#ifndef SCENE_H
#define SCENE_H

#include "public.h"
#include "scene_node.h"

OPEN_O2_NAMESPACE

class scScene
{
protected:
	scNode mRootNode;

public:
	scScene();
	~scScene();

	void update(float dt);
	void draw();

	shared<scNode> addNode(const shared<scNode>& node);
	bool removeNode(const shared<scNode>& node);
	bool removeAllNodes();
};

CLOSE_O2_NAMESPACE

#endif //SCENE_H
