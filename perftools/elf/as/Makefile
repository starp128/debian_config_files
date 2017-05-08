all: run_lib nolibc

run_lib:run_lib.c
	gcc -Wall -o run_lib -g -Llib/ -lterry run_lib.c

run_lib_nopic:run_lib.c
	gcc -Wall -o run_lib_nopic -g -Llib/ -lterry_nopic run_lib.c

nolibc: stubstart.S nolibc.c
	gcc -nostdlib stubstart.S -o nolibc  nolibc.c

clean:
	rm -f run_lib *.o