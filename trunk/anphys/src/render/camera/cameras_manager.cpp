#include "cameras_manager.h"

#include "camera.h"
#include "render_base_interface.h"

#include "../util/memory/mem_utils.h"

#include <algorithm>


grCamerasManager::grCamerasManager(grRenderBaseInterface* render):mRender(render)
{
}

grCamerasManager::~grCamerasManager()
{
	removeAllCameras();
}

grCamera* grCamerasManager::addCamera()
{
	mCameras.push_back(new grCamera());
	if (mCameras.size() == 1) mActiveCamera = *(mCameras.end() - 1);
	return *(mCameras.end() - 1);
}

grCamera* grCamerasManager::addCamera(grCamera* newCamera)
{
	mCameras.push_back(newCamera);
	if (mCameras.size() == 1) mActiveCamera = *(mCameras.end() - 1);
	return newCamera;
}

grCamera* grCamerasManager::setActiveCamera(grCamera* camera)
{
	mActiveCamera = camera;
	return mActiveCamera;
}

bool grCamerasManager::removeCamera(grCamera* camera)
{
	CamerasList::iterator it = std::find(mCameras.begin(), mCameras.end(), camera);

	if (it == mCameras.end()) return false;
	else mCameras.erase(it);

	return true;
}

bool grCamerasManager::removeAllCameras()
{
	for (CamerasList::iterator it = mCameras.begin(); it != mCameras.end(); it++)
		safe_release(*it);

	mCameras.clear();

	return true;
}
