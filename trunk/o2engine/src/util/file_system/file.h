#ifndef FILE_H
#define FILE_H

#include <fstream>

#include "public.h"
OPEN_O2_NAMESPACE

struct FileType
{
	enum value { FILE = 0, IMAGE, CONFIG, ATLAS };
};

/** Input file. */
class InFile
{
	std::ifstream mIfstream; /**< Input stream. */
	string        mFilename; /**< File name. */
	bool          mOpened;   /**< True, if file was opened. */

public:
	InFile();
	InFile(const string& filename, FileType::value type = FileType::FILE);
	~InFile();

	/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
	  * else file name will be combine with extension string from file system. */
	bool open(const string& filename, FileType::value type = FileType::FILE);

	/** Closing file. */
	bool close();
		               
	/** Read full file data and return size of readed data. */
	uint32 readFullData(void *dataPtr);

	/** Read data in dataPtr. */
	void readData(void *dataPtr, uint32 bytes);
		               
	/** Sets caret position. */
	void setCaretPos(uint32 pos);

	/** Return caret position. */
	uint32 getCaretPos();
		              
	/** Returns full data size. */
	uint32 getDataSize();
		               
	/** Returns true, if file was opened. */
	bool isOpened() const;

	/** Return file name. */
	const string& getFilename() const;
};

/** Output file. */
class OutFile
{
	std::ofstream mOfstream; /**< Output stream. */
	string        mFilename; /**< File name. */
	bool          mOpened;   /**< True, if file was opened. */

public:
	OutFile();
	OutFile(const string& filename, FileType::value type = FileType::FILE);
	~OutFile();

	/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
	  * else file name will be combine with first extension string from file system. */
	bool open(const string& filename, FileType::value type = FileType::FILE);

	/** Close file. */
	bool close();
		   
	/** Write some data from dataPtr. */
	void writeData(const void* dataPtr, uint32 bytes);
		               
	/** Returns true, if file was opened. */
	bool isOpened() const;

	/** Returns file name. */
	const string& getFilename() const;
};

CLOSE_O2_NAMESPACE

#endif //FILE_H