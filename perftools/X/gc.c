#include<stdio.h>
#include<stdlib.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xatom.h>
#include<X11/cursorfont.h>

int
main(int argc, char** args)
{
	int width = 600;
	int height = 400;
  
	Display* display = XOpenDisplay(0);

	if(!display) {
		printf("No display available\n");
		exit(1);
	}
  
	Window root = DefaultRootWindow(display);
	int defaultScreen = DefaultScreen(display);

	int screenBitDepth = 24;
	XVisualInfo visinfo = {};
	if(!XMatchVisualInfo(display, defaultScreen, screenBitDepth, TrueColor, &visinfo)) {
		printf("No matching visual info\n");
		exit(1);
	}

	XSetWindowAttributes windowAttr;
	windowAttr.background_pixel = 0xff0000;
	windowAttr.border_pixel = 0x00ff00;
	//windowAttr.bit_gravity
	//windowAttr.win_gravity
	windowAttr.colormap = XCreateColormap(display, root, visinfo.visual, AllocNone);
	unsigned long attributeMask = CWBackPixel | CWColormap | CWBorderPixel;

	Window window = XCreateWindow(display, root, 
				      0, 0,
				      width, height, 10,
				      visinfo.depth, InputOutput,
				      visinfo.visual, attributeMask, &windowAttr);

	if(!window) {
		printf("Window wasn't created properly\n");
		exit(1);
	}

	XFlush(display);
	// property
	XStoreName(display, window, "Hello, World!");
	XMapWindow(display, window);
	XFlush(display);

	/* draw somting */
	GC gr_context1;
	gr_context1=XCreateGC(display,window,0,NULL);
	XSetForeground(display, gr_context1, BlackPixel(display, DefaultScreen(display)));
	XDrawPoint(display, window, gr_context1, 100,100);
	XDrawLine(display, window, gr_context1, 1,1,100,100);
	XFlush(display);

	Cursor font_cursor;
	/* create a 'spider' cursor which is compiled in by default. */
	font_cursor = XCreateFontCursor(display, XC_spider);
	/* attach this cursor to our window. */
	XDefineCursor(display, window, font_cursor);
	XSync(display, False);

	XEvent evt;
	do{
		XNextEvent( display, &evt );   // calls XFlush()
	}while( evt.type != ButtonRelease );


	XDestroyWindow( display, window );
	XCloseDisplay( display );

	return 0;
} 
