#ifndef _FONT_H_INCLUDED_
#define _FONT_H_INCLUDED_

#include <X11/Xlib.h>

void FONT_Setup(Display* dpy);
void FONT_Write(Display* dpy, Window* bt, char* str, int x, int y);

#endif /* _FONT_H_INCLUDED_ */
