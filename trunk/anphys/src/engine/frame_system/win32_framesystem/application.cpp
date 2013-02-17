#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>

#include "application.h"
#include "windows_manager.h"
#include "simple_window.h"


void initializeWindowsSystem()
{
	gWindowsManager->initialize();
}

void runApplication()
{
	MSG  msg = tagMSG();
	while( msg.message != WM_QUIT) 
	{
		if(PeekMessage( &msg, NULL,0,0,PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else gWindowsManager->allUpdate();
	}
}

#endif