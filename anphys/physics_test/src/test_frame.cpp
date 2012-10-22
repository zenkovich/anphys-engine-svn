#include "test_frame.h"

#include "../../src/engine/engine_incl.h"

//debug tools
grRenderMulti3DObject* gRedPointsDebug;
grRenderMulti3DObject* gBluePointsDebug;
grRenderMulti3DObject* gGreenPointsDebug;
grRenderMulti3DObject* gRedArrowsDebug;
grRenderMulti3DObject* gBlueArrowsDebug;
grRenderMulti3DObject* gGreenArrowsDebug;


apTestFrame::apTestFrame():apRenderWindow()
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(vec2(0), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);
	mMainScene = NULL;

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect, fRect outputRect):apRenderWindow(title, wndRect, outputRect)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(outputRect.getSize(), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);
	mMainScene = NULL;

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect):apRenderWindow(title, wndRect)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(mInRect.getSize(), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);
	mMainScene = NULL;

	onCreate(mInRect);
}

apTestFrame::~apTestFrame()
{
	mRender->mCameras->removeCamera(mCamera3dMouse);
	mRender->mSceneManager->removeScene(mMainScene);
}

void apTestFrame::onCreate(fRect inRect)
{
	grLight* light = mRender->mLights->addLight(new grLight(NULL));
	light->initialize(grLightBaseInterface::light_directional_type, color4(1.0f, 1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,0), 0, 0, 0, 0, 0, 0, 0);
	light->setLightActive(true);

	createStdMaterials();
	createDebugTools();

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

	mMainScene = mRender->mSceneManager->addScene(new grRenderSceneBaseInterface(mRender->mSceneManager));
	/*grRender3DObjectMesh* obj = static_cast<grRender3DObjectMesh*>(mMainScene->mObjects->createObject(new grRender3DObjectMesh(NULL, vertCount, polyCount)));
	obj->addPart(verticies, vertCount, indexes, polyCount, smat);
	obj->mPosition = vec3(0,0,0);*/

	grRender3DObjectMesh* obj = new grRender3DObjectMesh(mMainScene->mObjects, vertCount, polyCount);
	obj->addPart(verticies, vertCount, indexes, polyCount, mRender->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));

	grRenderMulti3DObject* object = static_cast<grRenderMulti3DObject*>(mMainScene->mObjects->createObject(new grRenderMulti3DObject()));
	object->setRenderObject(obj);

	object->pushObject(grMultiVector(vec3(0), vec3(0, 0, 1)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(30), 0, 0), vec3(0, 4, 0)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(450), 0, rad(45)), vec3(0, 0, 30)));
	object->pushObject(new gr3DObjectConditionContainer(RotatedMatrix(rad(320), 0, 0), vec3(10, 0, 0)));


	safe_release_arr(verticies);
	safe_release_arr(indexes);
}

void apTestFrame::createStdMaterials()
{
	mRender->mMaterials->addMaterial(new grMaterial("redMaterial", color4(1.0f,0.0f,0.0f,1.0f), 
		color4(1.0f,0.0f,0.0f,1.0f), color4(1.0f,0.0f,0.0f,1.0f), color4(1.0f,0.0f,0.0f,1.0f), 1));
	mRender->mMaterials->addMaterial(new grMaterial("blueMaterial", color4(0.0f,0.0f,1.0f,1.0f), 
		color4(0.0f,0.0f,1.0f,1.0f), color4(0.0f,0.0f,1.0f,1.0f), color4(1.0f,0.0f,0.1f,1.0f), 1));
	mRender->mMaterials->addMaterial(new grMaterial("greenMaterial", color4(0.0f,1.0f,0.0f,1.0f), 
		color4(0.0f,1.0f,0.0f,1.0f), color4(0.0f,1.0f,0.0f,1.0f), color4(0.0f,1.0f,0.0f,1.0f), 1));

	grTexture* tex = mRender->mTextures->createTexture("../data/textures/white.jpg");

	grSurfaceMaterial* rmat = mRender->mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("redMaterial"));
	rmat->setMaterial(mRender->mMaterials->getMaterial("redMaterial"));
	rmat->pushTexture(tex);
	rmat->setShadeModel(NULL);

	grSurfaceMaterial* bmat = mRender->mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("blueMaterial"));
	bmat->setMaterial(mRender->mMaterials->getMaterial("blueMaterial"));
	bmat->pushTexture(tex);
	bmat->setShadeModel(NULL);

	grSurfaceMaterial* gmat = mRender->mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("greenMaterial"));
	gmat->setMaterial(mRender->mMaterials->getMaterial("greenMaterial"));
	gmat->pushTexture(tex);
	gmat->setShadeModel(NULL);

	mRender->mMaterials->addMaterial(new grMaterial("whiteMaterial", color4(1.0f,1.0f,1.0f,1.0f), 
		color4(1.0f,1.0f,1.0f,1.0f), color4(1.0f,1.0f,1.0f,1.0f), color4(1.0f,1.0f,1.0f,1.0f), 1));
}

void apTestFrame::createDebugTools()
{	
	mToolsScene = mRender->mSceneManager->addScene(new grRenderSceneBaseInterface(mRender->mSceneManager));

	vertexTexNorm* verticies = new vertexTexNorm[120];
	int* indexes = new int[360];
	int vertCount = 0;
	int polyCount = 0;

	arrowGeometry(verticies, vertCount, indexes, polyCount, vec3(1));

	gRedArrowsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gRedArrowsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gRedArrowsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));

	gGreenArrowsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gGreenArrowsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gGreenArrowsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("greenMaterial"));

	gBlueArrowsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gBlueArrowsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gBlueArrowsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("blueMaterial"));
	
	boxGeometry(verticies, vertCount, indexes, polyCount, vec3(0.1f));	

	gRedPointsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gRedPointsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gRedPointsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("redMaterial"));

	gGreenPointsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gGreenPointsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gGreenPointsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("greenMaterial"));

	gBluePointsDebug = static_cast<grRenderMulti3DObject*>(mToolsScene->mObjects->createObject(new grRenderMulti3DObject()));
	gBluePointsDebug->setRenderObject(new grRender3DObjectMesh(mToolsScene->mObjects, vertCount, polyCount));
	gBluePointsDebug->mRenderObject->addPart(verticies, vertCount, indexes, polyCount, 
		mRender->mSurfaceMaterials->getSurfaceMaterial("blueMaterial"));
	
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
