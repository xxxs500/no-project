#include <X11/Xft/Xft.h>
#include <unistd.h>
#include "font.h"

XftFont* xf;
XftColor color;

void FONT_Setup(Display* dpy) {
	xf = XftFontOpen(
			dpy, 0,
			XFT_FAMILY, XftTypeString, "VL ゴシック",
			XFT_SIZE, XftTypeDouble, 10.0, NULL);
	XftColorAllocName(dpy, DefaultVisual(dpy, 0), DefaultColormap(dpy, 0), "green", &color);
}

void FONT_Write(Display* dpy, Window* bt, char* str, int x, int y) {
	XftDraw* draw = XftDrawCreate(dpy, (*bt), DefaultVisual(dpy, 0), DefaultColormap(dpy, 0));
	XftDrawStringUtf8(draw, &color, xf, x, y, (FcChar8*)str, strlen(str));
}
