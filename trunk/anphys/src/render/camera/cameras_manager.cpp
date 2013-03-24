#include "cameras_manager.h"

#include <algorithm>

#include "camera.h"
#include "render/render_base_interface.h"
#include "util/memory/mem_utils.h"
#include "util/log/log_system.h"
#include "util/log/log_stream_in_file.h"


grCamerasManager::grCamerasManager(grRenderBaseInterface* render):mRender(render)
{
	mLog = gLogSystem->addStream(new cLogStreamInFile("cameras_log.txt"), "CamerasLog");
	mLog->mLogLevel = STD_LOG_LEVEL;
}

grCamerasManager::~grCamerasManager()
{
	removeAllCameras();
	gLogSystem->removeStream(mLog);
}

grCamera* grCamerasManager::addCamera()
{
	grCamera* newCamera = new grCamera();

	mCameras.push_back(newCamera);
	if (mCameras.size() == 1) mActiveCamera = newCamera;

	mLog->fout(1, "Added camera %x", newCamera);

	return newCamera;
}

grCamera* grCamerasManager::addCamera(grCamera* newCamera)
{
	mCameras.push_back(newCamera);
	if (mCameras.size() == 1) mActiveCamera = newCamera;

	mLog->fout(1, "Added camera %x", newCamera);

	return newCamera;
}

grCamera* grCamerasManager::setActiveCamera(grCamera* camera)
{
	mActiveCamera = camera;

	mLog->fout(1, "Activated camera %x", camera);

	return mActiveCamera;
}

bool grCamerasManager::removeCamera(grCamera* camera)
{
	CamerasList::iterator it = std::find(mCameras.begin(), mCameras.end(), camera);

	safe_release(camera);

	mLog->fout(1, "Removing camera %x", camera);

	if (it == mCameras.end()) return false;
	else mCameras.erase(it);

	return true;
}

bool grCamerasManager::removeAllCameras()
{
	for (CamerasList::iterator it = mCameras.begin(); it != mCameras.end(); it++)
		safe_release(*it);

	mCameras.clear();

	mLog->fout(1, "Removed all cameras");

	return true;
}
