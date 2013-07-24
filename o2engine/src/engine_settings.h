#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

/* 
 * Engine settings. 
 */

/* Using render system */
#define RENDER_OGL
//#define RENDER_DX9
//#define RENDER_GLES
//#define RENDER_GLES2

/* Using platform. */
#define PLATFORM_WIN
//#define PLATFORM_MAC
//#define PLATFORM_IOS

/* Log Level. 
 * 0 - not using log
 * 1 - using low log (only log() commands will be processed)
 * 2 - using hight log (all log commands will be processed) */
#define BASIC_LOG_LEVEL 2

/* Global log file. */
#define GLOBAL_LOG_FILENAME "log.txt"

#endif //ENGINE_SETTINGS_H