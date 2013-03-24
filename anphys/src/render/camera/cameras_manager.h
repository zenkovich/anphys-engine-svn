#ifndef CAMERAS_MANAGER_H
#define CAMERAS_MANAGER_H

#include <vector>

struct grCamera;
struct grRenderBaseInterface;
struct cLogStream;

struct grCamerasManager
{
	typedef std::vector<grCamera*> CamerasList;

	CamerasList            mCameras;
	grCamera*              mActiveCamera;

	grRenderBaseInterface* mRender;

	cLogStream*            mLog;

//functions
	grCamerasManager(grRenderBaseInterface* render);
	~grCamerasManager();

	grCamera* addCamera();
	grCamera* addCamera(grCamera* newCamera);

	grCamera* setActiveCamera(grCamera* camera);

	bool removeCamera(grCamera* camera);
	bool removeAllCameras();
};

#endif//CAMERAS_MANAGER_H