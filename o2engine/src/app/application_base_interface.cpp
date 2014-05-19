#include "application_base_interface.h"

#include "render_system/render_system.h"
#include "util/file_system/file_system.h"
#include "util/scheduler.h"
#include "util/time_utils.h"
#include "util/log.h"
#include "util/log/console_log_stream.h"
#include "util/log/file_log_stream.h"

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
	mInputMessage = mnew cInputMessage();

//file system
	mFileSystem = mnew cFileSystem();

//log
	cFileLogStream* fileLog = mnew cFileLogStream(BASIC_LOG_LEVEL, GLOBAL_LOG_FILENAME);
	gLog = mnew cConsoleLogStream(BASIC_LOG_LEVEL);
	fileLog->bindStream(gLog);
	mLog = mnew cFileLogStream("App", 2, "app_log.txt");
	gLog->bindStream(mLog);

	mLog->out("All Systems initialized");

//scheduler
	mScheduler = mnew cScheduler();

//timers
	mTimeUtils = mnew cTimeUtil();
}

void cApplicationBaseInterface::deinitializeSystems()
{
	safe_release(mFileSystem);
	safe_release(mScheduler);
	safe_release(mTimeUtils);

	mLog->out("All systems deinitialized");

	//safe_release(gLog->getParentStream());
}

shared<cInputMessage> cApplicationBaseInterface::getInputMessage()
{
	return mInputMessage;
}

shared<grRenderSystem> cApplicationBaseInterface::getRenderSystem() const
{
	return mRenderSystem;
}

CLOSE_O2_NAMESPACE