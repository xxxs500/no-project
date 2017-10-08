#include <string.h>
#include <stdlib.h>
#include "util.h"

int UTIL_CMP( const void *p, const void *q );

int UTIL_CMP( const void *p, const void *q ){
	return *(Window*)p - *(Window*)q;
}

unsigned long UTIL_RGB(Display *display, unsigned char r, unsigned char g, unsigned char b){
	Colormap cm;
	XColor xc;
	cm = DefaultColormap(display, DefaultScreen(display));
	xc.red = 257 * r;
	xc.green = 257 * g;
	xc.blue = 257 * b;
	XAllocColor(display, cm, &xc);
	return xc.pixel;
}

void UTIL_Focus(Display *dpy, Window root, Window w) {
	Window dum, p, *kids;
	unsigned int i, n;
	char *nam;

	if (UTIL_ReSearch(dpy, root, w) == 0 || XQueryTree(dpy, w, &dum, &p, &kids, &n) == 0) {
		XSetInputFocus(dpy, root, 0, CurrentTime);
		return;
	}
	if (p > root) w=p;
	for (i=0; i<n; i++) {
		XFetchName(dpy, kids[i], &nam);
		if (nam == NULL) continue;
		if (!strcmp(nam, "FocusProxy")) {
			w = kids[i];
			XFree(nam);
			break;
		}
		XFree(nam);
	}
	if (kids != NULL) XFree(kids);
	XSetInputFocus(dpy, w, 0, CurrentTime);
}

void UTIL_GetWindowIdFocus(Display *dpy, Window root, Window *w){
	unsigned int n;
	int m;
	Window now, par, dum, *kids;

	XGetInputFocus(dpy, &now, &m);
	if (now == None || now <= root) {
		(*w) = root;
		return;
	}
	XQueryTree(dpy, now, &dum, &par, &kids, &n);
	if (kids != NULL) XFree(kids);
	(*w) = (par > root) ? par : now;
}

void UTIL_MoveResize(Display *dpy, Window a, int x, int y, int w, int h){
	XRaiseWindow(dpy, a);
	XMoveWindow(dpy, a, x, y);
	XResizeWindow(dpy, a, w, h);
	XWarpPointer(dpy, None, a, 0, 0, 0, 0, 0, 0);
}

void UTIL_GetWindowIdNext(Display *dpy, Window root, Window *w){
	Window dum, *kids, act, first;
	XWindowAttributes att;
	int end;
	unsigned int i, n;
	char *nam;

	UTIL_GetWindowIdFocus(dpy, root, &act);

	XQueryTree(dpy, root, &dum, &dum, &kids, &n);
	qsort(kids, n, sizeof(Window), UTIL_CMP);
	for (i=0, end=0, first=0; i<n; i++) {
		XGetWindowAttributes(dpy, kids[i], &att);
		if (att.map_state != 2 || att.override_redirect != 0) continue;
		XFetchName(dpy, kids[i], &nam);
		if (nam != NULL) {
			if (!strcmp(nam, "panel")) {
				XFree(nam);
				continue;
			}
			XFree(nam);
		}
		if (first == 0) first = kids[i];
		if (end == 1) {
			(*w) = kids[i];
			XFree(kids);
			return;
		}
		if (kids[i] == act) end=1;
	}
	if (kids != NULL) XFree(kids);
	(*w) = (first == 0) ? root : first;
}

void UTIL_WarpPointer(Display *dpy, Window root, int x, int y){
	Window w;

	XWarpPointer(dpy, None, root, 0, 0, 0, 0, x, y);
	UTIL_GetWindowIdPointer(dpy, root, &w);
	UTIL_Focus(dpy, root, w);
}

void UTIL_GetWindowIdPointer(Display *dpy, Window root, Window *w){
	Window dum, kid;
	int n;
	unsigned int m;

	XQueryPointer(dpy, root, &dum, &kid, &n, &n, &n, &n, &m);
	(*w) = (kid == None) ? root : kid;
}

int UTIL_ReSearch(Display *dpy, Window root, Window itm) {
	Window dum, *kids;
	unsigned int i, n;

	if (XQueryTree(dpy, root, &dum, &dum, &kids, &n) == 0) return 0;
	for (i=0; i<n; i++) {
		if (kids[i] != itm) continue;
		XFree(kids);
		return 1;
	}
	if (kids != NULL) XFree(kids);
	return 0;
}
