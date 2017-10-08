#ifndef _HKEY_H_INCLUDE_
#define _HKEY_H_INCLUDE_

#include <X11/Xlib.h>
#include "util.h"

void HKEY_Setup(Display *dpy, Window root);
void HKEY_HotKey(Display *dpy, Window root, XEvent ev);

#endif /* _HKEY_H_INCLUDE_ */

