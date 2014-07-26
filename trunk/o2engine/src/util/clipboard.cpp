#include "clipboard.h"

#ifdef PLATFORM_WIN
#include <Windows.h>
#endif //PLATFORM_WIN

OPEN_O2_NAMESPACE

void setClipboardText(const string& text)
{
	if(OpenClipboard(NULL))
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		EmptyClipboard(); 
		hgBuffer= GlobalAlloc(GMEM_DDESHARE, text.length() + 1);
		chBuffer= (char*)GlobalLock(hgBuffer); 
		strcpy(chBuffer, text.c_str());
		GlobalUnlock(hgBuffer);
		SetClipboardData(CF_TEXT, hgBuffer);
		CloseClipboard(); 
	}
}

string getClipboardText()
{
	string res;

	if (OpenClipboard(NULL))
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		char* chBuffer= (char*)GlobalLock(hData);
		res = chBuffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}

	return res;
}


CLOSE_O2_NAMESPACE