#include <X11/Xlib.h>
#include <string.h>
#include "util.h"
#include "hkey.h"

int main() {
	Display *dpy;
	Window root, dum;
	XEvent ev;
	XWindowAttributes att;
	char *nam;
	unsigned long color0, color1;

	if (!(dpy = XOpenDisplay(0x0))) return 1;
	root = DefaultRootWindow(dpy);

	color0 = UTIL_RGB(dpy, 0, 0, 0);
	color1 = UTIL_RGB(dpy, 128, 255, 255);

	HKEY_Setup(dpy, root);

	XSelectInput( dpy, root, SubstructureNotifyMask );

	for (;;) {
		XNextEvent(dpy, &ev);
		switch (ev.type) {
			case KeyPress:
				HKEY_HotKey(dpy, root, ev);
				break;

			case MapNotify:
				XFetchName(dpy, ev.xmap.window, &nam);
				if (nam != NULL) {
					if (!strcmp(nam, "panel")) {
						XFree(nam);
						break;
					}
					XFree(nam);
				}
				XGetWindowAttributes(dpy, ev.xmap.window, &att);
				if (att.map_state != 2 || att.override_redirect != 0) break;
				XSetWindowBorderWidth(dpy, ev.xmap.window, 1);
				XSelectInput( dpy, ev.xmap.window, FocusChangeMask );
				UTIL_Focus(dpy, root, ev.xmap.window);
				break;

			case FocusOut:
				if (UTIL_ReSearch(dpy, root, ev.xfocus.window) == 0) break;
				XSetWindowBorder(dpy, ev.xfocus.window, color0);
				break;

			case FocusIn:
				if (UTIL_ReSearch(dpy, root, ev.xfocus.window) == 0) break;
				XSetWindowBorder(dpy, ev.xfocus.window, color1);
				break;

			case UnmapNotify:
				UTIL_GetWindowIdPointer(dpy, root, &dum);
				UTIL_Focus(dpy, root, dum);
				break;
		}
	}
}

