all: libterry.so libterry_nopic.so

libterry.so:libterry.c
	gcc -g -fPIC -shared -o libterry.so libterry.c


libterry_nopic.so:libterry.c
	gcc -g -shared -o libterry_nopic.so libterry.c

