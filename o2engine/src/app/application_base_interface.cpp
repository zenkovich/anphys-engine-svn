#include "application_base_interface.h"

#include "render_system/render_system.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/log/console_log_stream.h"
#include "util/log/file_log_stream.h"

OPEN_O2_NAMESPACE

cApplicationBaseInterface::cApplicationBaseInterface():
	mLogStream(NULL), mRenderSystem(NULL), mFileSystem(NULL)
{
	initalizeSystems();
}

cApplicationBaseInterface::~cApplicationBaseInterface()
{
	deinitializeSystems();
}

void cApplicationBaseInterface::initalizeSystems()
{
//file system
	mFileSystem = new cFileSystem;

//log
	cFileLogStream* fileLog = new cFileLogStream(BASIC_LOG_LEVEL, GLOBAL_LOG_FILENAME);
	gLog = new cConsoleLogStream(BASIC_LOG_LEVEL);
	fileLog->bindStream(gLog);
	mLogStream = new cFileLogStream("App", 2, "app_log.txt");
	gLog->bindStream(mLogStream);

//render system
	mRenderSystem = new grRenderSystem;
}

void cApplicationBaseInterface::deinitializeSystems()
{
	gLog->unbindStream(mLogStream);

	safe_release(mRenderSystem);

	safe_release(mFileSystem);
}

CLOSE_O2_NAMESPACE