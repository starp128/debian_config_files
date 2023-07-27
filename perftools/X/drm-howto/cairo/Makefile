
FLAGS=`pkg-config --cflags --libs libdrm`
FLAGS+=`pkg-config --cflags --libs cairo`


# modeset13_cairo:modeset13_cairo.c
# 	gcc modeset13_cairo.c -o modeset13_cairo $(FLAGS)


# link to custom built cairo without X
# build cairo with following configuration
# ./configure --disable-xlib --disable-xlib-xrender --disable-xcb --disable-xcb-shm --without-x --disable-ft --disable-ps --disable-svg --disable-fc --disable-png
modeset13_cairo2:modeset13_cairo.c
	gcc modeset13_cairo.c -o modeset13_cairo2 -g -O0 `pkg-config --cflags --libs libdrm` -I/b/ws/04/run/qemu_desktop/docs/drm-howto/cairo/cairo/src -L/b/ws/04/run/qemu_desktop/docs/drm-howto/cairo/cairo/src -lcairo
