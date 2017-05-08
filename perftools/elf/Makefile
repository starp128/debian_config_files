obj=elf.o binary_dump.o
all:$(obj)
	gcc -g -o elf $(obj)

elf.o : elf.c
	gcc -g -c -o elf.o elf.c

binary_dump.o :binary_dump.c
	gcc -g -c -o binary_dump.o binary_dump.c

elfdig: elfdig.c elfstring.c
	gcc -g -o elfdig elfstring.c elfdig.c

clean:
	rm -f *.o elf elfdig
