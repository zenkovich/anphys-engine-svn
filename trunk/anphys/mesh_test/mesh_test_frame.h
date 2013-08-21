#ifndef MESH_TEST_FRAME_H
#define MESH_TEST_FRAME_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

struct grSimple3DRenderState;
struct grCamera3DMouse;
struct gr2DRenderState;
struct uiWidgetsManager;
struct cScene;

class apMeshTestFrame:public apRenderWindow
{
	grSimple3DRenderState*        m3DRenderState;
	grCamera3DMouse*              mCamera;
	cScene*                       mMainEngineScene;

	gr2DRenderState*              m2DRenderState;

	uiWidgetsManager*             mWidgetsManager;

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
};

#endif //MESH_TEST_FRAME_H