#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include "public.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class cApplication;

class cDeviceInfo:public cSingleton<cDeviceInfo>
{
public:
	enum DeviceParam { DP_RENDER_TARGET_AVAILABLE = 0,
	                   DP_RENDER_MAX_TEXTURE_SIZE,   
					   DP_DEVICE_ID,

					   DEVICE_PARAMETRES_COUNT };
	enum { nParamDataSize = 128 };

protected:
	char mDeviceParametres[DEVICE_PARAMETRES_COUNT][nParamDataSize];

public:
	cDeviceInfo();
	~cDeviceInfo();

	void initialize(cApplication* app);

	template<typename T>
	T getParameter(DeviceParam paramId);
};

#define deviceInfo() cDeviceInfo::instance()

CLOSE_O2_NAMESPACE

#endif //DEVICE_INFO_H