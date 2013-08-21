#ifndef PUBLIC_H
#define PUBLIC_H

/* 
 * Engine public file.
 * Contains basic stuff using in engine. 
 */

#include "engine_settings.h"

/** Opening engine namespace macros. You can redefine it. */
#define OPEN_O2_NAMESPACE namespace o2 {

/** Closing engine namespace macros. You can redefine it. */
#define CLOSE_O2_NAMESPACE }

/** Safe release object. */
#define safe_release(obj) { if (obj != 0) delete obj; }

/** Safe release array object. */
#define safe_release_arr(obj) { if (obj != 0) delete[] obj; }

//some includes
#include "util/assert.h"
#include <string>

//types
typedef unsigned long long uint64;
typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef long long          int64;

//some macros
#ifdef PLATFORM_WIN
#	define isWinPlatform() true
#	define isMacPlatform() false
#	define isIOSPlatform() false
#	define isPCPlatform() true
#	define isTabletPlatform() false
#	define isPhonePlatform() false
#endif

//add windows libraries
#if defined(PLATFORM_WIN) && defined(RENDER_OGL)
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	pragma comment(lib, "odbc32.lib")
#	pragma comment(lib, "odbccp32.lib")
#elif defined(PLATFORM_WIN) && defined(RENDER_D3D9C)
#endif

#endif //PUBLIC_H