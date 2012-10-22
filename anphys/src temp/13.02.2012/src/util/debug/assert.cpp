#include "assert.h"

#ifdef PLATFORM_WIN32

#include <windows.h>
#include "../other/string_utils.h"

void errorMessage(const std::string& desc, const std::string& file, long line)
{
	std::string message = "Error:\n" + desc + "\nat:\n" + file + " : " + toStr((int)line);

	*gLog << message.c_str();

	if (MessageBox(NULL, message.c_str(), "Error", MB_OK | MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL) == IDCANCEL)
		ExitProcess ((UINT)-1);
}

#endif //PLATFORM_WIN32
