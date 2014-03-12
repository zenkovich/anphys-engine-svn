#ifndef APPLICATION_H
#define APPLICATION_H

/* !!! More information about cApplication interface see in application_base_interface.h */

#include "engine_settings.h"

#if defined(PLATFORM_WIN)
	#include "win/application_win.h"
#endif

OPEN_O2_NAMESPACE

inline cApplication* application() { return (cApplication*)(cApplication::instancePtr()); }
inline cInputMessage* appInput() { return application()->getInputMessage(); }

CLOSE_O2_NAMESPACE

#endif //APPLICATION_H