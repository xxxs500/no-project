#ifndef _UTIL_H_INCLUDE_
#define _UTIL_H_INCLUDE_

#include <X11/Xlib.h>

unsigned long UTIL_RGB(Display *display, unsigned char r, unsigned char g, unsigned char b);
void UTIL_Focus(Display *dpy, Window root, Window w);
void UTIL_GetWindowIdFocus(Display *dpy, Window root, Window *w);
void UTIL_MoveResize(Display *dpy, Window a, int x, int y, int w, int h);
void UTIL_GetWindowIdNext(Display *dpy, Window root, Window *w);
void UTIL_WarpPointer(Display *dpy, Window root, int x, int y);
void UTIL_GetWindowIdPointer(Display *dpy, Window root, Window *w);
int UTIL_ReSearch(Display *dpy, Window root, Window itm);

#endif /* _UTIL_H_INCLUDE_ */
