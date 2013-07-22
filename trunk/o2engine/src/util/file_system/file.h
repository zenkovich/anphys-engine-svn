#ifndef FILE_H
#define FILE_H

#include <fstream>

#include "public.h"
OPEN_O2_NAMESPACE

struct cFileType
{
	enum value { FT_FILE = 0, FT_IMAGE, FT_CONFIG };
};

class cInFile
{
	std::ifstream mIfstream;
	std::string   mFilename;
	bool          mOpened;

public:
	cInFile();
	cInFile(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	~cInFile();

	bool               open(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	bool               close();
		               
	uint64             readFullData(void *dataPtr);
	void               readData(void *dataPtr, uint64 bytes);
		               
	void               setCaretPos(uint64 pos);
	uint64             getCaretPos();
		               
	uint64             getDataSize();
		               
	bool               isOpened() const;

	const std::string& getFilename() const;
};

class cOutFile
{
	std::ofstream mOfstream;
	std::string   mFilename;
	bool          mOpened;

public:
	cOutFile();
	cOutFile(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	~cOutFile();

	bool               open(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	bool               close();
		               
	void               writeData(void* dataPtr, uint64 bytes);
		               
	bool               isOpened() const;

	const std::string& getFilename() const;
};

CLOSE_O2_NAMESPACE

#endif //FILE_H