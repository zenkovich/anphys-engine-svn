#pragma once

#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include "windows_manager.h"

extern apWindowsManager *gWindowsManager;

void initializeWindowsSystem();

//запустить приложение
void runApplication();

#endif