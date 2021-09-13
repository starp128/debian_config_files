/*
gcc -o xim_example xim_example.c -lX11
gcc -o xim_example xim_example.c -lX11 -L/opt/lib/ -g -O0
LD_LIBRARY_PATH=/opt/lib/
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <assert.h>

Display *dpy;
Window win;
int scr;

void send_spot(XIC ic, XPoint nspot) {
	XVaNestedList preedit_attr;
	preedit_attr = XVaCreateNestedList(0, XNSpotLocation, &nspot, NULL);
	XSetICValues(ic, XNPreeditAttributes, preedit_attr, NULL);
	XFree(preedit_attr);
}

int main() {
	char *p;


	if (!XSupportsLocale()) {
		perror("not support locale\n");
		return -1;
	}

	
	p = setlocale(LC_CTYPE, "zh_CN.utf8");
	if (p == NULL) {
		perror("can not set locale\n");
		return -1 ;
	}
	XSetLocaleModifiers("@im=fcitx");


/* inside main, after XSetLocaleModifiers */
/* setting up a simple window */
	dpy = XOpenDisplay(NULL);
	scr = DefaultScreen(dpy);
	win = XCreateSimpleWindow(dpy,
				  XDefaultRootWindow(dpy),
				  0, 0, 100, 100, 5,
				  BlackPixel(dpy, scr),
				  BlackPixel(dpy, scr));
	XMapWindow(dpy, win);

/* initialize IM and IC */
	XIM xim = XOpenIM(dpy, NULL, NULL, NULL);
	XIC ic = XCreateIC(xim,
/* the following are in attr, val format, terminated by NULL */
			   XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
			   XNClientWindow, win,
			   NULL);
/* focus on the only IC */
	XSetICFocus(ic);
/* capture the input */
	XSelectInput(dpy, win, KeyPressMask);


	static char *buff;
	size_t buff_size = 16;
	buff = (char *)malloc(buff_size);
	for (;;)
	{
		KeySym ksym;
		Status status;
		XEvent ev;
		XPoint spot;
		XNextEvent(dpy, &ev);
		if (XFilterEvent(&ev, None))
			continue;
		if (ev.type == KeyPress)
		{
			size_t c = Xutf8LookupString(ic, &ev.xkey,
						     buff, buff_size - 1,
						     &ksym, &status);
			if (status == XBufferOverflow)
			{
				printf("reallocate to the size of: %lu\n", c + 1);
				buff = realloc(buff, c + 1);
				c = Xutf8LookupString(ic, &ev.xkey,
						      buff, c,
						      &ksym, &status);
			}
			if (c)
			{
				spot.x += 20;
				spot.y += 20;
				send_spot(ic, spot);
				buff[c] = 0;
				printf("delievered string: %s\n", buff);
			}
		}
	}

}

