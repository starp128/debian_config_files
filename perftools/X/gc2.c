#include<stdio.h>
#include<stdlib.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xatom.h>

int main(){

	Display *display = XOpenDisplay( NULL );
	if (!display) {
		return 1;
	}

	int screenNumber = DefaultScreen(display);
	unsigned long white = WhitePixel(display,screenNumber);
	unsigned long black = BlackPixel(display,screenNumber);

	Window win = XCreateSimpleWindow(display,
					 DefaultRootWindow(display),
					 50, 50,   // origin
					 200, 200, // size
					 0, black, // border
					 white );  // backgd

	XMapWindow( display, win );

	long eventMask = StructureNotifyMask;
	XSelectInput( display, win, eventMask );

	XEvent evt;
	/* do{ */
	/* 	XNextEvent( display, &evt );   // calls XFlush */
	/* }while( evt.type != MapNotify ); */

	XFlush(display);

	GC gc = XCreateGC( display, win,
			   0,        // mask of values
			   NULL );   // array of values
	XSetForeground( display, gc, black );


	XDrawLine(display, win, gc, 10, 10,190,190); //from-to
	XDrawLine(display, win, gc, 10,190,190, 10); //from-to
	//XDrawLine(display, win, gc, 0,0,100,100);

	eventMask = ButtonPressMask|ButtonReleaseMask;
	XSelectInput(display,win,eventMask); // override prev

	do{
		XNextEvent(display, &evt );   // calls XFlush()
	}while( evt.type != ButtonRelease );


	XDestroyWindow( display, win );
	XCloseDisplay( display );

	return 0;
}

