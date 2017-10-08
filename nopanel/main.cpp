#include <X11/Xlib.h>
#include <vector>
#include <iostream>
#include <string>
#include "font.h"

using namespace std;

struct icons{
	Window wid;
	Window bid;
};
unsigned long RGB(Display *display, unsigned char r, unsigned char g, unsigned char b);
void MakeIcoList(Display *dpy, Window root);
void AddIcoList(Display *dpy, Window root, Window add);
void DelIcoList(Window del);
void ClearIco(Display *dpy);
void ViewIco(Display *dpy, Window root, Window w);
void GetWindowIdFocus(Display *dpy, Window root, Window *w);
int ReSearch(Display *dpy, Window root, Window itm);
void Focus(Display *dpy, Window root, Window w);

vector<icons> ico;
unsigned long COLOR0;
unsigned long COLOR1;

int main ()
{
	Display *dpy;
	Window w, root, dum;
	XEvent ev;
	int scr;
	char *nam;

	dpy = XOpenDisplay(NULL);
	scr = DefaultScreen(dpy);
	root = RootWindow(dpy, scr);

	COLOR0 = RGB(dpy, 128, 128, 128);
	COLOR1 = RGB(dpy, 255, 255, 255);

	w = XCreateSimpleWindow(dpy, root, 0, 1040, 1700, 30, 1, 0, 0);
	XStoreName(dpy, w, "panel");
	XSetIconName(dpy, w, "panel");
	XMapWindow(dpy, w);

	FONT_Setup(dpy);
	XSelectInput(dpy, root, SubstructureNotifyMask);

	MakeIcoList(dpy, root);
	ViewIco(dpy, root, w);

	while(1){
		XNextEvent(dpy, &ev);
		switch (ev.type) {
			
			case FocusIn:
				ClearIco(dpy);
				ViewIco(dpy, root, w);
				break;
				
			case UnmapNotify:
				ClearIco(dpy);
				DelIcoList(ev.xmap.window);
				ViewIco(dpy, root, w);
				break;
				
			case MapNotify:
				ClearIco(dpy);
				AddIcoList(dpy, root, ev.xmap.window);
				ViewIco(dpy, root, w);
				break;

			case ButtonPress:
				if (ev.xbutton.window == None) break;
				XGetIconName(dpy, ev.xbutton.window, &nam);
				dum = atol(nam);
				if (ev.xbutton.button == 1) {
					XRaiseWindow(dpy, dum);
					Focus(dpy, root, dum);
				} else if (ev.xbutton.button == 2) {
					XDestroyWindow(dpy, dum);
				}
				break;

			default:
				break;
		}
	}

	XCloseDisplay(dpy);
	return 0;
}

unsigned long RGB(Display *display, unsigned char r, unsigned char g, unsigned char b){
	Colormap cm;
	XColor xc;
	cm = DefaultColormap(display, DefaultScreen(display));
	xc.red = 257 * r;
	xc.green = 257 * g;
	xc.blue = 257 * b;
	XAllocColor(display, cm, &xc);
	return xc.pixel;
}

void MakeIcoList(Display *dpy, Window root) {
	Window dum, *kids;
	unsigned int i, n;

	XQueryTree(dpy, root, &dum, &dum, &kids, &n);
	for (i=0; i<n; i++) {
		AddIcoList(dpy, root, kids[i]);
	}
	if (kids != NULL) XFree(kids);
}

void AddIcoList(Display *dpy, Window root, Window add) {
	XWindowAttributes att;
	icons ic;
	char *nam;

	if (ReSearch(dpy, root, add) == 0) return;
	XGetWindowAttributes(dpy, add, &att);
	if (att.map_state != 2 || att.override_redirect != 0) return;
	XGetIconName(dpy, add, &nam);
	if (nam != NULL){
		if (string(nam)=="panel") {
			XFree(nam);
			return;
		}
		XFree(nam);
	}
	ic.wid = add;
	ic.bid = 0;
	ico.push_back(ic);
}

void DelIcoList(Window del) {
	unsigned int i;

	for (i=0; i<(unsigned int)ico.size(); i++) {
		if (ico[i].wid != del) continue;
		ico.erase(ico.begin()+i);
		break;
	}
}

void ClearIco(Display *dpy) {
	unsigned int i;

	for (i=0; i<(unsigned int)ico.size(); i++) {
		if (ico[i].bid > 0) XDestroyWindow(dpy, ico[i].bid);
	}
}

void ViewIco(Display *dpy, Window root, Window w) {
	Window bt, now;
	unsigned int i, j;
	unsigned long color;
	char *nam;
	vector<Window> err;

	GetWindowIdFocus(dpy, root, &now);

	for (i=0, j=0; i<(unsigned int)ico.size(); i++){
		if (j > 9) break;
		if (ReSearch(dpy, root, ico[i].wid) == 0) {
			err.push_back(ico[i].wid);
			continue;
		}
		XGetIconName(dpy, ico[i].wid, &nam);
		XSelectInput(dpy, ico[i].wid, FocusChangeMask);
		color = (ico[i].wid == now) ? COLOR1 : COLOR0;
		bt = XCreateSimpleWindow(dpy, w, (j*150), 0, 146, 25, 1, color, 0);
		ico[i].bid = bt;
		XSetIconName(dpy, bt, to_string(ico[i].wid).c_str());
		XMapWindow(dpy, bt);
		XSelectInput(dpy, bt, ButtonPressMask);
		FONT_Write(dpy, &bt, (nam != NULL) ? nam : (char*)"*****", 4, 20);
		if (nam != NULL) XFree(nam);
		j++;
	}

	for (i=0; i<(unsigned int)err.size(); i++) DelIcoList(err[i]);
}

void GetWindowIdFocus(Display *dpy, Window root, Window *w){
	unsigned int n;
	int m;
	Window now, par, dum, *kids;

	XGetInputFocus(dpy, &now, &m);
	if (now == None || now <= root) {
		(*w) = root;
		return;
	}
	do {
		XQueryTree(dpy, now, &dum, &par, &kids, &n);
		if (kids != NULL) XFree(kids);
		(*w) = now;
		now = par;
	} while (par > root);
}

int ReSearch(Display *dpy, Window root, Window itm) {
	Window dum, *kids;
	unsigned int i, m;

	XQueryTree(dpy, root, &dum, &dum, &kids, &m);
	for (i=0; i<m; i++) {
		if (kids[i] != itm) continue;
		XFree(kids);
		return 1;
	}
	if (kids != NULL) XFree(kids);
	return 0;
}

void Focus(Display *dpy, Window root, Window w) {
	Window dum, p, *kids;
	unsigned int i, n;
	char *nam;

	if (ReSearch(dpy, root, w) == 0) {
		XSetInputFocus(dpy, root, 0, CurrentTime);
		return;
	}
	XQueryTree(dpy, w, &dum, &p, &kids, &n);
	if (p > root) w=p;
	for (i=0; i<n; i++) {
		XFetchName(dpy, kids[i], &nam);
		if (nam == NULL) continue;
		if (string(nam) == "FocusProxy") {
			w = kids[i];
			XFree(nam);
			break;
		}
		XFree(nam);
	}
	if (kids != NULL) XFree(kids);
	XSetInputFocus(dpy, w, 0, CurrentTime);
}
