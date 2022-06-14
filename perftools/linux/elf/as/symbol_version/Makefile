main:main.c
	gcc -Wl,--version-script link.map main.c -o $@


libsymver.so: libsymver.c
	gcc -shared -fPIC -o $@ -Wl,--version-script symver.map $^

try.so: libsymver.c
	gcc -shared -fPIC -o $@ $^

testlib:main.c
	gcc main.c -o $@ -L. -lsymver
