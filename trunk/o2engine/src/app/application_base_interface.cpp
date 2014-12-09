#include "application_base_interface.h"

#include <time.h>
#include "config/project_config.h"
#include "render_system/render_system.h"
#include "ui/ui_controller.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/log/console_log_stream.h"
#include "util/log/file_log_stream.h"
#include "util/scheduler.h"
#include "util/time_utils.h"
#include "util/timer.h"
#include "assets/assets.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cApplicationBaseInterface);

cApplicationBaseInterface::cApplicationBaseInterface():
	mLog(NULL), mRenderSystem(NULL), mFileSystem(NULL)
{
	initalizeSystems();
}

cApplicationBaseInterface::~cApplicationBaseInterface()
{
	deinitializeSystems();
}

void cApplicationBaseInterface::initalizeSystems()
{
	srand(time(NULL));

//log
	cFileLogStream* fileLog = mnew cFileLogStream(BASIC_LOG_LEVEL, GLOBAL_LOG_FILENAME);
	gLog = mnew cConsoleLogStream(BASIC_LOG_LEVEL);
	fileLog->bindStream(gLog);
	mLog = mnew cFileLogStream("App", 2, "app_log.txt");
	gLog->bindStream(mLog);

	mLog->out("All Systems initialized");

//file system
	mFileSystem = mnew cFileSystem();

//project config
	mProjectConfig = mnew ProjectConfig();

//assets
	mAssets = mnew Assets();

//input message
	mInputMessage = mnew cInputMessage();

//scheduler
	mScheduler = mnew cScheduler();

//timer
	mTimer = mnew cTimer();
	mTimer->reset();

//timers
	mTimeUtils = mnew cTimeUtil();

//ui
	mUIController = mnew uiController();
}

void cApplicationBaseInterface::deinitializeSystems()
{
	safe_release(mFileSystem);
	safe_release(mScheduler);
	safe_release(mTimeUtils);
	safe_release(mUIController);
	safe_release(mProjectConfig);
	safe_release(mAssets);

	mLog->out("All systems deinitialized");

	//safe_release(gLog->getParentStream());
}

void cApplicationBaseInterface::processFrame()
{	
	float dt = clamp(mTimer->getElapsedTime(), 0.001f, 0.1f);

	mTimeUtils->update(dt);

	mScheduler->processBeforeFrame(dt);

	onUpdate(dt);
	mUIController->update(dt);

	mRenderSystem->beginRender();
	onDraw();
	mUIController->draw();
	mRenderSystem->endRender();

	mInputMessage->update(dt);

	mScheduler->processAfterFrame(dt);
}

cInputMessage* cApplicationBaseInterface::getInputMessage()
{
	return mInputMessage;
}

grRenderSystem* cApplicationBaseInterface::getRenderSystem() const
{
	return mRenderSystem;
}

cLogStream* cApplicationBaseInterface::getLog() const
{
	return mLog;
}

ProjectConfig* cApplicationBaseInterface::getProjectConfig() const
{
	return mProjectConfig;
}

cFileSystem* cApplicationBaseInterface::getFileSystem() const
{
	return mFileSystem;
}

cScheduler* cApplicationBaseInterface::getScheduler() const
{
	return mScheduler;
}

cTimeUtil* cApplicationBaseInterface::getTimeUtils() const
{
	return mTimeUtils;
}

uiController* cApplicationBaseInterface::getUIController() const
{
	return mUIController;
}

Assets* cApplicationBaseInterface::getAssets() const
{
	return mAssets;
}

CLOSE_O2_NAMESPACE