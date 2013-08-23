#ifndef MESH_TEST_FRAME_H
#define MESH_TEST_FRAME_H

#include "engine/engine_options.h"

#include <vector>

#ifdef PLATFORM_WIN32
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

struct grSimple3DRenderState;
struct grCamera3DMouse;
struct gr2DRenderState;
struct uiWidgetsManager;
struct cScene;
struct grRender3DObjectMesh;
struct uiWindow;
struct uiWidget;

class cMeshTest;

class apMeshTestFrame:public apRenderWindow
{
	typedef std::vector<grRender3DObjectMesh*> MeshVec;

	grSimple3DRenderState* m3DRenderState;
	grCamera3DMouse*       mCamera;
	cScene*                mMainEngineScene;

	gr2DRenderState*       m2DRenderState;

	uiWidgetsManager*      mWidgetsManager;
	int                    mWidgetsManagerRes;

	cMeshTest*             mMeshTest;
	MeshVec                mMainMeshes;
	grRender3DObjectMesh*  mSecondaryMesh;

	mat3x3                 mLastCameraOrient;
	vec3                   mLastCameraPos;

	uiWindow*              mMainWindow;
	uiWidget*              mPlaneParams;
	uiWidget*              mTorusParams;
	uiWidget*              mSphereParams;

	vec3                   mMainMeshSize;
	vec3                   mMainMeshRandomize;
	int                    mMainMeshXSegments;
	int                    mMainMeshZSegments;

	vec3                   mSecondaryMeshSize;
	vec3                   mSecondaryMeshRandomize;
	int                    mSecondaryMeshXSegments;
	int                    mSecondaryMeshZSegments;



public:
	apMeshTestFrame();
	apMeshTestFrame(const std::string& title, fRect wndRect, fRect outputRect);
	apMeshTestFrame(const std::string& title, fRect wndRect);
	~apMeshTestFrame();

	void onCreate(fRect inRect);
	void onClose();
	float onTimer();

	void onSize(fRect inRect);

	void onMouseMove(vec2 point);	
	void onKeyDown(int key);

private:
	void updateCameraContorls();

	void initializeUI();

	void createMaterials();

	void updateSecMeshPositioning();

	void resetMainMesh();
};

#endif //MESH_TEST_FRAME_H