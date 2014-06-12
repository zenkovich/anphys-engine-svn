#ifndef PUBLIC_PLATFORMS_H 
#define PUBLIC_PLATFORMS_H

#include "public_namespace.h"

OPEN_O2_NAMESPACE
	
#ifdef PLATFORM_WIN
#	define isWinPlatform() true
#	define isMacPlatform() false
#	define isIOSPlatform() false
#	define isPCPlatform() true
#	define isTabletPlatform() false
#	define isPhonePlatform() false
#endif

CLOSE_O2_NAMESPACE

#endif //PUBLIC_PLATFORMS_H
