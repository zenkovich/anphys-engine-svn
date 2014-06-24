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

scNode* scScene::addNode(scNode* node)
{
	return mRootNode.addNode(node);
}

bool scScene::removeNode(scNode* node)
{
	return mRootNode.removeNode(node);
}

bool scScene::removeAllNodes()
{
	return mRootNode.removeAllNodes();
}

CLOSE_O2_NAMESPACE