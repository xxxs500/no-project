#include "hkey.h"

#define MK_S4 (ShiftMask|Mod4Mask)
#define MK_1  (Mod1Mask)
#define MK_4  (Mod4Mask)

int XK_TB;
int XK_C;
int XK_F;
int XK_J;
int XK_K;
int XK_U;
int XK_I;
int XK_O;
int XK_P;
int XK_H;

void HKEY_Setup(Display *dpy, Window root){
	XK_TB = XKeysymToKeycode(dpy, XStringToKeysym("Tab"));
	XK_C  = XKeysymToKeycode(dpy, XStringToKeysym("c"));
	XK_F  = XKeysymToKeycode(dpy, XStringToKeysym("f"));
	XK_J  = XKeysymToKeycode(dpy, XStringToKeysym("j"));
	XK_K  = XKeysymToKeycode(dpy, XStringToKeysym("k"));
	XK_U  = XKeysymToKeycode(dpy, XStringToKeysym("u"));
	XK_I  = XKeysymToKeycode(dpy, XStringToKeysym("i"));
	XK_O  = XKeysymToKeycode(dpy, XStringToKeysym("o"));
	XK_P  = XKeysymToKeycode(dpy, XStringToKeysym("p"));
	XK_H  = XKeysymToKeycode(dpy, XStringToKeysym("h"));

	XGrabKey(dpy, XK_TB, MK_1,  root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_C,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_F,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_J,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_K,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_U,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_I,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_O,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_P,  MK_S4, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_U,  MK_4,  root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_I,  MK_4,  root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_O,  MK_4,  root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_P,  MK_4,  root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, XK_H,  MK_4,  root, True, GrabModeAsync, GrabModeAsync);
}

void HKEY_HotKey(Display *dpy, Window root, XEvent ev){
	Window act, dum, w;
	int i;
	unsigned int n, width, height;
	
	XGetGeometry(dpy, root, &dum, &i, &i, &width, &height, &n, &n);
	UTIL_GetWindowIdFocus(dpy, root, &act);

	switch (ev.xkey.state) {
		case MK_S4:
			if (act <= root) {
				break;
			} else if (ev.xkey.keycode == XK_C) {
				XDestroyWindow(dpy, act);
			} else if (ev.xkey.keycode == XK_F) {
				UTIL_MoveResize(dpy, act, 0, 0, width-2, height-52);
			} else if (ev.xkey.keycode == XK_J) {
				UTIL_MoveResize(dpy, act, 0, 0, width/2-2, height-52);
			} else if (ev.xkey.keycode == XK_K) {
				UTIL_MoveResize(dpy, act, width/2, 0, width/2-2, height-52);
			} else if (ev.xkey.keycode == XK_U) {
				UTIL_MoveResize(dpy, act, 0, 0, width/2-2, (height-50)/2-2);
			} else if (ev.xkey.keycode == XK_I) {
				UTIL_MoveResize(dpy, act, width/2, 0, width/2-2, (height-50)/2-2);
			} else if (ev.xkey.keycode == XK_O) {
				UTIL_MoveResize(dpy, act, 0, (height-50)/2, width/2-2, (height-50)/2-2);
			} else if (ev.xkey.keycode == XK_P) {
				UTIL_MoveResize(dpy, act, width/2, (height-50)/2, width/2-2, (height-50)/2-2);
			}
			break;

		case MK_4:
			if (ev.xkey.keycode == XK_U) {
				UTIL_WarpPointer(dpy, root, 0, 0);
			} else if (ev.xkey.keycode == XK_I) {
				UTIL_WarpPointer(dpy, root, width/2, 0);
			} else if (ev.xkey.keycode == XK_O) {
				UTIL_WarpPointer(dpy, root, 0, (height-50)/2);
			} else if (ev.xkey.keycode == XK_P) {
				UTIL_WarpPointer(dpy, root, width/2, (height-50)/2);
			} else if (ev.xkey.keycode == XK_H) {
				XLowerWindow(dpy, act);
				UTIL_GetWindowIdPointer(dpy, root, &w);
				UTIL_Focus(dpy, root, w);
			}
			break;

		case MK_1:
			if (ev.xkey.keycode == XK_TB) {
				UTIL_GetWindowIdNext(dpy, root, &w);
				XRaiseWindow(dpy, w);
				XWarpPointer(dpy,None,w,0,0,0,0,0,0);
				UTIL_Focus(dpy, root, w);
			}
			break;
	}
}
