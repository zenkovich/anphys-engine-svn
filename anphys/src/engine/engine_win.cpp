#include "engine_options.h"

#ifdef PLATFORM_WIN32

#include "engine.h"
#include "engine/frame_system/render_frame.h"
#include "engine/frame_system/win32_framesystem/windows_manager.h"
#include "engine/frame_system/win32_framesystem/simple_window.h"
#include "engine/frame_system/win32_framesystem/application.h"
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"


cEngine::~cEngine()
{
	safe_release(mLog);
	safe_release(mFileSystem);

	for (RenderFramesList::iterator it = mFrames.begin(); it != mFrames.end(); ++it)
	{
		safe_release(*it);
	}
}

void cEngine::initialize()
{
	mLog = new cLogStreamInFile("engine_log.txt");
	mLog->mLogLevel = INITIAL_ENGINE_LOG_LEVEL;
	gLogSystem->addStream(mLog, "Engine");


	mLog->fout(1, "Initializing engine...");

	cFileSystem::initializeSingleton();

	initializeWindowsSystem();

	mLog->fout(1, "initialization complete!");
}

void cEngine::run()
{
	mLog->fout(1, "RUN\n");

	runApplication();
}

grRenderFrame* cEngine::addWindowFrame(const std::string& title, fRect rect)
{
	mLog->fout(1, "Add window frame: title '%s' rect (%.2f %.2f %.2f)", title.c_str(), rect.leftTop.x, rect.leftTop.y,
		                                                                      rect.rightDown.x, rect.rightDown.y);

	grRenderFrame* newFrame = static_cast<apRenderWindow*>(gWindowsManager->addWindow(new apRenderWindow(title, rect)));
	newFrame->mEngine = this;
	mFrames.push_back(newFrame);
	return newFrame;
}
	
grRenderFrame* cEngine::addFrame(grRenderFrame* newFrame)
{
	mLog->fout(1, "Add window frame %x: wnd name '%s'", newFrame, newFrame->mWndName.c_str());

	apWindow* window = static_cast<apWindow*>(newFrame);
	grRenderFrame* createNewFrame = static_cast<apRenderWindow*>(gWindowsManager->addWindow(window));
	createNewFrame->mEngine = this;
	mFrames.push_back(createNewFrame);
	return newFrame;
}
	
void cEngine::exitEngine()
{
	mLog->fout(1, "Exit");

	exit(0);
}


#endif //PLATFORM_WIN32