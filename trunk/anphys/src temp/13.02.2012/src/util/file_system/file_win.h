#ifndef FILE_WIN_H
#define FILE_WIN_H

#include <fstream>

struct cFileIdentity
{
	std::ifstream mInFile;
	std::ofstream mOutFile;

	bool mIsInput;
};

#endif //FILE_WIN_H
