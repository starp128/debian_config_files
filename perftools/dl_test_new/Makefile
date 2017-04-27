all:
	gcc -g -Wall -fPIC -c run_loop.c
	gcc -shared -Wl,-soname,librun_loop.so -o librun_loop.so run_loop.o
	gcc -Wall test_run_loop.c -L`pwd`/ -lrun_loop -o test_run_loop -g -ldl

test_run_loop:
	gcc -Wall test_run_loop.c -o test_run_loop -g -ldl

#copy librun_loop.so to dynamic libary path.

clean:
	rm -rf run_loop.o librun_loop.so test_run_loop
