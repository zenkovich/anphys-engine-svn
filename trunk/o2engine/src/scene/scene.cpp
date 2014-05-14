#include "scene.h"

OPEN_O2_NAMESPACE

scScene::scScene()
{
}

scScene::~scScene()
{
}

void scScene::update(float dt)
{

}

void scScene::draw()
{

}

shared(scNode) scScene::addNode(const scNode& node)
{
	mRootNode.addNode(node);
}

bool scScene::removeNode(const shared(scNode)& node)
{
	return mRootNode.removeNode(node);
}

bool scScene::removeAllNodes()
{
	return mRootNode.removeAllNodes();
}

CLOSE_O2_NAMESPACE