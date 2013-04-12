#include "box_collision_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "physics/CD/collision_detection/box_box_cd.h"
#include "physics/CD/collision_geometry/box_collision_geometry.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"

#include "input/input_messenger.h"

apBoxCollisionTestFrame::apBoxCollisionTestFrame():apRenderWindow(), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = new grCamera3DMouse;

	onCreate(mInRect);
}

apBoxCollisionTestFrame::apBoxCollisionTestFrame(const std::string& title, fRect wndRect, fRect outputRect):
	apRenderWindow(title, wndRect, outputRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = new grCamera3DMouse;

	onCreate(mInRect);
}

apBoxCollisionTestFrame::apBoxCollisionTestFrame(const std::string& title, fRect wndRect):
	apRenderWindow(title, wndRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = new grCamera3DMouse;

	onCreate(mInRect);
}

apBoxCollisionTestFrame::~apBoxCollisionTestFrame()
{
	safe_release(m3DRenderState);
	safe_release(mCamera3dMouse);
	safe_release(mMainEngineScene);
}

void apBoxCollisionTestFrame::onCreate(fRect inRect)
{
	m3DRenderState = new grSimple3DRenderState(mRender);
	mRender->bindRenderState(m3DRenderState);
	m3DRenderState->bindCamera(mCamera3dMouse);

	//create main scene
	mMainEngineScene = new cScene(this);

	grLight* light = mRender->mLights->addLight(new grLight(NULL));
	light->initialize(grLightBaseInterface::light_directional_type, color4(1.0f, 1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,0), 0, 0, 0, 0, 0, 0, 0);
	light->setLightActive(true);

	//mTestBox = mMainEngineScene->addObject(mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0, 0, 0), vec3(1, 1, 1)));
	//mTestBox->getPhysicsRigidBody()->applyImpulse(vec3(0, 0, 1), vec3(0, 10, 0));
	
	mBoxAPos = vec3(0.0f, 0.0f, 0.0f);
	mBoxASize = vec3(10.0f, 1.0f, 10.0f);
	mBoxBAngles = vec3(0.0f, 0.0f, 0.0f);
	
	mBoxBPos = vec3(0.5f, 1.0f, 0.3f);
	mBoxBSize = vec3(1.0f, 1.0f, 1.0f);
	mBoxBAngles = vec3(0.0f, 0.0f, 0.0f);

	mBoxBObject = mBoxAObject = NULL;

	testBoxCD();

	mPhysicsRunning = false;
}

float apBoxCollisionTestFrame::onTimer()
{
	float dt = apRenderWindow::onTimer();

	if (isKeyDown(key_w)) mCamera3dMouse->moveForward(isKeyDown(key_shift));
	if (isKeyDown(key_s)) mCamera3dMouse->moveBack(isKeyDown(key_shift));
	if (isKeyDown(key_a)) mCamera3dMouse->moveLeft(isKeyDown(key_shift));
	if (isKeyDown(key_d)) mCamera3dMouse->moveRight(isKeyDown(key_shift));
	if (isKeyDown(key_ctrl)) mCamera3dMouse->moveDown(isKeyDown(key_shift));
	if (isKeyDown(key_space)) mCamera3dMouse->moveUp(isKeyDown(key_shift));

	
	if (mPhysicsRunning) 
	{
		//getRenderStuff().reset();		
	}
	mRender->update(dt);
	mMainEngineScene->update(dt);

	mRender->beginRender();
	mRender->render();
	mRender->endRender();


	if (mPhysicsRunByStep) mPhysicsRunning = false;

	return dt;
}

void apBoxCollisionTestFrame::onClose()
{
	mEngine->exitEngine();
}

void apBoxCollisionTestFrame::onSize(fRect inRect)
{
}

void apBoxCollisionTestFrame::onMouseLeftButtonDown(vec2 point)
{
}

void apBoxCollisionTestFrame::onMouseLeftButtonUp(vec2 point)
{
}

void apBoxCollisionTestFrame::onMouseRightButtonDown(vec2 point)
{
}

void apBoxCollisionTestFrame::onMouseRightButtonUp(vec2 point)
{
}

void apBoxCollisionTestFrame::onMouseMove(vec2 point)
{
	if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON))
		mCamera3dMouse->mouseMoved(point - mInputMessenger->mInputMessage.mCursorPosition);
}

void apBoxCollisionTestFrame::onMouseWheel(float delta)
{
}

void apBoxCollisionTestFrame::onKeyDown(int key)
{
	*gLog << formatStr("key = %i\n", key).c_str();
	
	if (key == key_t) mPhysicsRunning = !mPhysicsRunning;
	if (key == key_y) mPhysicsRunByStep = !mPhysicsRunByStep;

	if (key == key_u)
	{
		if (isKeyDown(key_shift)) mBoxAPos.x -= 0.03f;
		else                         mBoxBPos.x -= 0.03f;
		testBoxCD();
	}

	if (key == key_i)
	{
		if (isKeyDown(key_shift)) mBoxAPos.x += 0.03f;
		else                         mBoxBPos.x += 0.03f;
		testBoxCD();
	}

	if (key == key_j)
	{
		if (isKeyDown(key_shift)) mBoxAPos.y -= 0.03f;
		else                         mBoxBPos.y -= 0.03f;
		testBoxCD();
	}

	if (key == key_k)
	{
		if (isKeyDown(key_shift)) mBoxAPos.y += 0.03f;
		else                         mBoxBPos.y += 0.03f;
		testBoxCD();
	}

	if (key == key_n)
	{
		if (isKeyDown(key_shift)) mBoxAPos.z -= 0.03f;
		else                         mBoxBPos.z -= 0.03f;
		testBoxCD();
	}

	if (key == key_m)
	{
		if (isKeyDown(key_shift)) mBoxAPos.z += 0.03f;
		else                         mBoxBPos.z += 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_7)
	{
		if (isKeyDown(key_shift)) mBoxASize.x -= 0.03f;
		else                         mBoxBSize.x -= 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_8)
	{
		if (isKeyDown(key_shift)) mBoxASize.x += 0.03f;
		else                         mBoxBSize.x += 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_4)
	{
		if (isKeyDown(key_shift)) mBoxASize.y -= 0.03f;
		else                         mBoxBSize.y -= 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_5)
	{
		if (isKeyDown(key_shift)) mBoxASize.y += 0.03f;
		else                         mBoxBSize.y += 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_1)
	{
		if (isKeyDown(key_shift)) mBoxASize.z -= 0.03f;
		else                         mBoxBSize.z -= 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_2)
	{
		if (isKeyDown(key_shift)) mBoxASize.z += 0.03f;
		else                         mBoxBSize.z += 0.03f;
		testBoxCD();
	}

	if (key == key_numpad_3)
	{
		float d = 1.0f;
		if (isKeyDown(key_numpad_0)) d = -d;
		if (isKeyDown(key_shift)) mBoxAAngles.z += d;
		else                         mBoxBAngles.z += d;
		testBoxCD();
	}

	if (key == key_numpad_6)
	{
		float d = 1.0f;
		if (isKeyDown(key_numpad_0)) d = -d;
		if (isKeyDown(key_shift)) mBoxAAngles.y += d;
		else                         mBoxBAngles.y += d;
		testBoxCD();
	}

	if (key == key_numpad_9)
	{
		float d = 1.0f;
		if (isKeyDown(key_numpad_0)) d = -d;
		if (isKeyDown(key_shift)) mBoxAAngles.x += d;
		else                         mBoxBAngles.x += d;
		testBoxCD();
	}

	if (key == key_numpad_0)
	{
		testBoxCD();
	}
}

void apBoxCollisionTestFrame::onKeyUp(int key)
{

	if (key == key_numpad_0)
	{
		testBoxCD();
	}
}

void apBoxCollisionTestFrame::onActive()
{
}

void apBoxCollisionTestFrame::onDeActive()
{
}
/*
void apTestFrame::testConstraintSolve( float dt, const vec3& attachPoint )
{
	//vec3 attachPoint(0.5f, 0.5f, 0.5f);
	vec3 sceneAttachPoint(0);
	float maxLength = 4.0f;

	if (dt < 0.000001) dt = 0.001f;

	phRigidObject* rigidObject = mTestBox->getPhysicsRigidBody();
	vec3 worldAttachPoint = attachPoint*rigidObject->mOrient + rigidObject->mPosition;

	vec3 diff = worldAttachPoint - sceneAttachPoint;
	float diffLength = diff.len();
	vec3 normDiff = diff/diffLength;

	vec3 pointSpeed = rigidObject->mVelocity + (rigidObject->mAngularVelocity^(worldAttachPoint - rigidObject->mPosition));

	float pointSpeedNormalProj = pointSpeed*normDiff;

	float pen = diffLength - maxLength;

	float fr = 0;
	if (pen > 0 && pointSpeedNormalProj > 0) fr = 1.0f;

	float force = fmax(0.0f, ((pointSpeedNormalProj + pen/dt*0.3f)*rigidObject->mMass*2.0f*0.7f)*fr )/2.0f;
	vec3 forceImpulse = normDiff*-force;

	*gLog << formatStr("speed before impulse %.2f %.2f %.2f (%.2f)\n", pointSpeed.x, pointSpeed.y, pointSpeed.z, pointSpeed.len());

	rigidObject->applyImpulse(worldAttachPoint, forceImpulse);

	pointSpeed = rigidObject->mVelocity + (rigidObject->mAngularVelocity^(worldAttachPoint - rigidObject->mPosition));
	*gLog << formatStr("speed after impulse %.2f %.2f %.2f (%.2f)\n", pointSpeed.x, pointSpeed.y, pointSpeed.z, pointSpeed.len());

	*gLog << formatStr("impulse %.3f %.3f %.3f (%.3f)\n", forceImpulse.x, forceImpulse.y, forceImpulse.z, forceImpulse);

	//dbg
	
	getRenderStuff().addRedCube(worldAttachPoint);
	getRenderStuff().addRedArrow(worldAttachPoint, worldAttachPoint + forceImpulse*0.1f);
	getRenderStuff().addBlueArrow(sceneAttachPoint, worldAttachPoint);
	getRenderStuff().addGreenArrow(worldAttachPoint, worldAttachPoint + pointSpeed*dt*2.0f);
}*/

void apBoxCollisionTestFrame::testBoxCD()
{
	getRenderStuff().reset();
		
	/*getRenderStuff().addRedArrow(vec3(0), vec3(10, 0, 0));
	getRenderStuff().addGreenArrow(vec3(0), vec3(0, 10, 0));
	getRenderStuff().addBlueArrow(vec3(0), vec3(0, 0, 10));*/

	if (mBoxAObject) mMainEngineScene->removeObject(mBoxAObject);
	if (mBoxBObject) mMainEngineScene->removeObject(mBoxBObject);
	
	mat3x3 rotationMatrixA = RotatedMatrix(rad(mBoxAAngles.x), rad(mBoxAAngles.y), rad(mBoxAAngles.z));
	mat3x3 rotationMatrixB = RotatedMatrix(rad(mBoxBAngles.x), rad(mBoxBAngles.y), rad(mBoxBAngles.z));
	
	phRigidObject* objectA = new phRigidObject;
	phRigidObject* objectB = new phRigidObject;

	phCollisionGeometry* geomA = new phCollisionGeometry;
	phCollisionGeometry* geomB = new phCollisionGeometry;

	objectA->mCollisionGeometry = geomA;
	geomA->mObject = objectA;

	objectB->mCollisionGeometry = geomB;
	geomB->mObject = objectB;

	phBoxCollisionGeometry* boxAGeom = new phBoxCollisionGeometry(rotationMatrixA, mBoxAPos, mBoxASize);
	phBoxCollisionGeometry* boxBGeom = new phBoxCollisionGeometry(rotationMatrixB, mBoxBPos, mBoxBSize);
	
	geomA->addPart(boxAGeom);
	geomB->addPart(boxBGeom);
	
	geomA->preUpdate(0);
	geomB->preUpdate(0);
	geomA->update(0);
	geomB->update(0);

	phCollision collision;

	checkCollisionBoxBox(boxAGeom, boxBGeom, &collision);
	
	vec3 offs;
	if (collision.mPoints->size() > 0 && isKeyDown(key_numpad_0)) 
		offs = (*collision.mPoints->mValues[0]).mNormal*(*collision.mPoints->mValues[0]).mDepth*-0.5f;

	mBoxAObject = mMainEngineScene->addObject(mMainEngineScene->mSceneStuff->createBoxMesh(mBoxAPos + offs, mBoxASize, rotationMatrixA));
	
	mBoxBObject = mMainEngineScene->addObject(mMainEngineScene->mSceneStuff->createBoxMesh(mBoxBPos - offs, mBoxBSize, rotationMatrixB));
	
	static int edges[12][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
		                        { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 },
								{ 6, 7 }, { 7, 4 }, { 4, 5 }, { 5, 6 }	};

	/*for (int i = 0; i < 12; i++)
	{
		getRenderStuff().addRedArrow(boxAGeom->mPoints[edges[i][0]],   boxAGeom->mPoints[edges[i][1]]);
		getRenderStuff().addGreenArrow(boxBGeom->mPoints[edges[i][0]], boxBGeom->mPoints[edges[i][1]]);
		
		getRenderStuff().addRedArrow(boxAGeom->mPoints[edges[i][0]] + offs,   boxAGeom->mPoints[edges[i][1]] + offs);
		getRenderStuff().addGreenArrow(boxBGeom->mPoints[edges[i][0]] - offs, boxBGeom->mPoints[edges[i][1]] - offs);
	}*/
}
