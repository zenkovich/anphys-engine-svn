#include "device_info.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cDeviceInfo);

cDeviceInfo::cDeviceInfo()
{
}

cDeviceInfo::~cDeviceInfo()
{
}

template<typename T>
T cDeviceInfo::getParameter( DeviceParam paramId )
{
	return (*(T*)mDeviceParametres[clamp<int>(paramId, 0, DEVICE_PARAMETRES_COUNT - 1)]);
}

void cDeviceInfo::initialize( cApplication* app )
{
//device id
	if (isWinPlatform())
		strcpy(mDeviceParametres[DP_DEVICE_ID], "Windows");

//render target available
	(*(bool*)&mDeviceParametres[DP_RENDER_TARGET_AVAILABLE]) = app->mRenderSystem->isRenderTargetAvailable();

//max texture size
	(*(vec2i*)&mDeviceParametres[DP_RENDER_MAX_TEXTURE_SIZE]) = app->mRenderSystem->getMaxTextureSize();
}


CLOSE_O2_NAMESPACE