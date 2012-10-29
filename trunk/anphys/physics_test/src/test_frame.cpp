#include "test_frame.h"

#include "../../src/engine/engine_incl.h"

apTestFrame::apTestFrame():apRenderWindow(), mMainEngineScene(NULL)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(vec2(0), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect, fRect outputRect):
	apRenderWindow(title, wndRect, outputRect), mMainEngineScene(NULL)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(outputRect.getSize(), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect):apRenderWindow(title, wndRect), mMainEngineScene(NULL)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(mInRect.getSize(), 
		                                                                                            mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::~apTestFrame()
{
	mRender->mCameras->removeCamera(mCamera3dMouse);
	mRender->mSceneManager->removeScene(mToolsScene);

	safe_release(mMainEngineScene);
}

void apTestFrame::onCreate(fRect inRect)
{
	//create main scene
	mMainEngineScene = new cScene(this);

	grLight* light = mRender->mLights->addLight(new grLight(NULL));
	light->initialize(grLightBaseInterface::light_directional_type, color4(1.0f, 1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,0), 0, 0, 0, 0, 0, 0, 0);
	light->setLightActive(true);

	grTexture* tex = mRender->mTextures->createTexture("../data/textures/wood.jpg");
	grSurfaceMaterial* smat = mRender->mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial());
	smat->setMaterial(mRender->mMaterials->getMaterial("whiteMaterial"));
	smat->pushTexture(tex);
	smat->setShadeModel(NULL);

	vertexTexNorm* verticies = new vertexTexNorm[120];
	int* indexes = new int[360];
	int vertCount = 0;
	int polyCount = 0;

	arrowGeometry(verticies, vertCount, indexes, polyCount, vec3(1));
	//boxGeometry(verticies, vertCount, indexes, polyCount, vec3(1));

	/*grRender3DObjectMesh* obj = static_cast<grRender3DObjectMesh*>(mMainScene->mObjects->createObject(new grRender3DObjectMesh(NULL, vertCount, polyCount)));
	obj->addPart(verticies, vertCount, indexes, polyCount, smat);
	obj->mPosition = vec3(0,0,0);*/

	/*grRender3DObjectMesh* obj = new grRender3DObjectMesh(mMainScene->mObjects, vertCount, polyCount);
	obj->addPart(verticies, vertCount, indexes, polyCount, mRender->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));

	grRenderMulti3DObject* object = static_cast<grRenderMulti3DObject*>(mMainScene->mObjects->createObject(new grRenderMulti3DObject()));
	object->setRenderObject(obj);

	object->pushObject(grMultiVector(vec3(0), vec3(0, 0, 1)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(30), 0, 0), vec3(0, 4, 0)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(450), 0, rad(45)), vec3(0, 0, 30)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(320), 0, 0), vec3(10, 0, 0)));*/


	safe_release_arr(verticies);
	safe_release_arr(indexes);
}

float apTestFrame::onTimer()
{
	float dt = apRenderWindow::onTimer();

	if (isKeyPressed(key_w)) mCamera3dMouse->moveForward(isKeyPressed(key_shift));
	if (isKeyPressed(key_s)) mCamera3dMouse->moveBack(isKeyPressed(key_shift));
	if (isKeyPressed(key_a)) mCamera3dMouse->moveLeft(isKeyPressed(key_shift));
	if (isKeyPressed(key_d)) mCamera3dMouse->moveRight(isKeyPressed(key_shift));
	if (isKeyPressed(key_ctrl)) mCamera3dMouse->moveDown(isKeyPressed(key_shift));
	if (isKeyPressed(key_space)) mCamera3dMouse->moveUp(isKeyPressed(key_shift));

	mRender->update(dt);
	mRender->preRender();
	mRender->render();
	mRender->postRender();

	return dt;
}

void apTestFrame::onClose()
{
	mEngine->exitEngine();
}

void apTestFrame::onSize(fRect inRect)
{
	mCamera3dMouse->mScreenSize = inRect.getSize();
}

void apTestFrame::onMouseLeftButtonDown(vec2 point)
{
}

void apTestFrame::onMouseLeftButtonUp(vec2 point)
{
}

void apTestFrame::onMouseRightButtonDown(vec2 point)
{
}

void apTestFrame::onMouseRightButtonUp(vec2 point)
{
}

void apTestFrame::onMouseMove(vec2 point)
{
	if (mLeftMouseButton) mCamera3dMouse->mouseMove(point - mCursorPos);
}

void apTestFrame::onMouseWheel(float delta)
{
}

void apTestFrame::onKeyDown(int key)
{
	*gLog << formatStr("key = %i\n", key).c_str();
}

void apTestFrame::onKeyUp(int key)
{
}

void apTestFrame::onActive()
{
}

void apTestFrame::onDeActive()
{
}
