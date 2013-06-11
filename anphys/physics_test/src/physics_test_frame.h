#ifndef PHYSICS_TEST_FRAME_H
#define PHYSICS_TEST_FRAME_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

#include "vehicle/vehicle.h"

struct grCamera3DMouse;
struct cScene;
struct cObject;
struct grSimple3DRenderState;
struct gr2DRenderState;
struct grCamera2D;
struct grCamera3D;
struct uiWidgetsManager;
struct uiWindow;
struct LandscapeCreatorWnd;
struct VehicleCreatorWidnow;
struct phLandscapeCollisionGeometry;

struct TestLandscape
{
	physics::lVertex*  mVertexBuffer;
	physics::lPolygon* mPolygonsBuffer;
	unsigned int       mVertexCount;
	unsigned int       mPolygonsCount;
	
	physics::lPolygon** mTestPolygonsBuffer;
	unsigned int        mTestPolygonsBufferSize;
	unsigned int        mTestPolygonsBufferCount;

//functions
	TestLandscape();
	~TestLandscape();

	void resizeBuffers(unsigned int vertexCount, unsigned int polygonsCount);
	void resizeTestPolyBuffer(unsigned int size);

	void getPolygons(const physics::vec3& minv, const physics::vec3& maxv);
};

struct apPhysicsTestFrame:public apRenderWindow
{
	grCamera3DMouse*              mCamera3dMouse;
	grCamera2D*                   m2DCamera;
	grCamera3D*                   mVehicleCamera;
	grSimple3DRenderState*        m3DRenderState;
	gr2DRenderState*              m2DRenderState;
								  
	cScene*                       mMainEngineScene;
								  
	bool                          mPhysicsRunning;
	bool                          mPhysicsRunByStep;

	bool                          mFollowCameraVehicle;
	vec3                          mVehicleCamPos;
	vec3                          mVehicleCamTargetPos;
								  
//ui							  
	uiWidgetsManager*             mWidgetsManager;
	int                           mWidgetsRes;
	uiWindow*                     mMainMenuWindow;
								  
	LandscapeCreatorWnd*          mLandscapeCreator;
	VehicleCreatorWidnow*         mVehicleCreator;
								  
//physics						  
	cObject*                      mVehicleObject;
	cObject*                      mLandscapeObject;

	physics::Vehicle              mVehicle;
	TestLandscape                 mTestLandscapeGeom;

//functions
	apPhysicsTestFrame();
	apPhysicsTestFrame(const std::string& title, fRect wndRect, fRect outputRect);
	apPhysicsTestFrame(const std::string& title, fRect wndRect);
	~apPhysicsTestFrame();

	void onCreate(fRect inRect);
	void onClose();
	float onTimer();

	void onSize(fRect inRect);

	void onMouseMove(vec2 point);	
	void onKeyDown(int key);

protected:
	void onOpenLandscapeCreatorBtnPressed();
	void onOpenVehicleCreatorBtnPressed();

	void updateVehicle(float dt);

	void updateCameraControls();

	void render2D();
	void setupScene1();
	void createLandscapeObject();
	void createVehicleObject();
	void createUIWidgets();
};


#endif //PHYSICS_TEST_FRAME_H