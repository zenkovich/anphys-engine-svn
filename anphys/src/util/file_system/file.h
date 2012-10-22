#ifndef FILE_H
#define FILE_H

#include "../../engine/engine_options.h"
#ifdef PLATFORM_WIN32
#include "file_win.h"
#endif


struct cFile: public cFileBase
{	
	bool			mReady;	
	unsigned int	mRefCount;
	
	cFile();
	cFile(const char* fileName, FileType type = FT_INPUT);
	~cFile();
	
	bool openRead(const char* fileName);
	bool openWrite(const char* fileName);

	void close();
};

#endif //FILE_H