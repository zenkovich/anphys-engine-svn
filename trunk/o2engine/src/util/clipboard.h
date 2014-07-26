#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "public.h"

OPEN_O2_NAMESPACE

/** Sets the clipboard data text. */
void setClipboardText(const string& text);

/** Returns clipboard data as text. */
string getClipboardText();

CLOSE_O2_NAMESPACE

#endif //CLIPBOARD_H
