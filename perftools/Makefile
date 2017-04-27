task=t loop signal_demo encrypt constructor_demo hello hello_withno_libc
subdirs=$(shell ls -d */)

.PHONY: all
all:$(task) $(subdirs)

.PHONY: $(subdirs)
$(subdirs):
	@if [ -f $@/Makefile ] ; then \
	  $(MAKE) -C $@ -f Makefile; \
	else \
	    echo "Not find Makefile in "$@; \
	fi \

t:t.c
	gcc -g -o $@ $<

time:time.c
	gcc -g -o $@ $<

loop:loop.c
	gcc -g -o $@ $<

signal_demo:signal_demo.c
	gcc -g -o $@ $<

hello_withno_libc:hello_withno_libc_start.S hello_withno_libc.c
	gcc -nostdlib hello_withno_libc_start.S hello_withno_libc.c -o hello_withno_libc

encrypt:encrypt.c
	gcc -g -o $@ $< -lcrypt

constructor_demo: constructor_demo.c
	gcc -g -o $@ $<

.PHONY: clean
clean:
	rm -rf $(task) a.out *.o
	@for dir in $(subdirs); do \
	  if [ -f $$dir/Makefile ] ; then \
	    $(MAKE) -C $$dir -f Makefile clean; \
	  else \
	    echo "Not find Makefile in "$$dir; \
	  fi \
	done
