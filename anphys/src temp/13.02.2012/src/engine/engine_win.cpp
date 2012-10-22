#include "engine_options.h"

#ifdef PLATFORM_WIN32

#include "engine.h"
#include "render_system/render_frame.h"
#include "../win32_platform/windows_manager.h"
#include "../win32_platform/simple_window.h"
#include "../win32_platform/application.h"
#include "render_system/render_frame_win32_window/render_frame_win32_window.h"

void cEngine::initialize()
{
	initializeWindowsSystem();
}

void cEngine::run()
{
	runApplication();
}

grRenderFrame* cEngine::addWindowFrame(const std::string& title, fRect rect)
{
	grRenderFrame* newFrame = static_cast<apRenderWindow*>(gWindowsManager->addWindow(new apRenderWindow(title, rect)));
	newFrame->mEngine = this;
	mFrames.push_back(newFrame);
	return newFrame;
}
	
grRenderFrame* cEngine::addFrame(grRenderFrame* newFrame)
{
	apWindow* window = static_cast<apWindow*>(newFrame);
	grRenderFrame* createNewFrame = static_cast<apRenderWindow*>(gWindowsManager->addWindow(window));
	createNewFrame->mEngine = this;
	mFrames.push_back(createNewFrame);
	return newFrame;
}
	
void cEngine::exitEngine()
{
	exit(0);
}


#endif //PLATFORM_WIN32