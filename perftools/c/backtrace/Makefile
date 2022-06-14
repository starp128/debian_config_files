backtrace: backtrace.c
	gcc backtrace.c -o backtrace -rdynamic 

clean:
	rm -rf *.o backtrace


# -rdynamic
#     Pass the flag -export-dynamic to the ELF linker, on targets that support it. This instructs the linker to add all symbols, not only used ones, to the dynamic
#     symbol table. This option is needed for some uses of "dlopen" or to allow obtaining backtraces from within a program.
