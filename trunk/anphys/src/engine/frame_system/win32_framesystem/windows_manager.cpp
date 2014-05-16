#include "engine/engine_options.h"


#ifdef PLATFORM_WIN32
#include "windows_manager.h"
#include "simple_window.h"
#include "util/log/log_system.h"
#include "util/log/log_stream_in_file.h"

extern apWindowsManager *gWindowsManager = new apWindowsManager;
extern cLogSystem* gLogSystem;

LRESULT apWindowsManager::procWindows(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	for (std::vector<apWindow*>::iterator it = mWindows.begin(); it != mWindows.end(); it++)
	{
		if (wnd == (*it)->mHWnd)
		{
			return (*it)->windowProc(uMsg, wParam, lParam);
		}
	}

	return NULL;
}

void apWindowsManager::allUpdate()
{
	for (std::vector<apWindow*>::iterator it = mWindows.begin(); it != mWindows.end(); it++)
	{
		(*it)->onTimer();
	}
}

void apWindowsManager::initialize()
{
	mLog = gLogSystem->addStream(new cLogStreamInFile("windows_manager_log.txt"), "windowsManager");
	mLog->mLogLevel = INITIAL_WINDOWS_MANAGER_LOG_LEVEL;
}

apWindow* apWindowsManager::addWindow(apWindow* window)
{
	window->mpManager = this;
	mWindows.push_back(window);

	mLog->fout(1, "added window[%i] '%s'", mWindows.size() - 1, window->mWndName.c_str());

	return mWindows[mWindows.size() - 1];
}

apWindow* apWindowsManager::addStdWindow(fRect windowRect, std::string title)
{
	apWindow *newWnd = new apWindow;
	mWindows.push_back(newWnd);

	RECT rt;
	rt.left = (LONG)windowRect.leftTop.x;
	rt.right = (LONG)windowRect.rightDown.x;
	rt.top = (LONG)windowRect.leftTop.y;
	rt.bottom = (LONG)windowRect.rightDown.y;

	newWnd->initializeStd(NULL, title, rt);
	newWnd->mpManager = this;	
	
	mLog->fout(1, "added std window[%i] '%s'", mWindows.size() - 1, newWnd->mWndName.c_str());

	return mWindows[mWindows.size() - 1];
}

apWindowsManager::apWindowsManager()
{
}

apWindowsManager::~apWindowsManager()
{
	gLogSystem->removeStream(mLog);
}

#endif