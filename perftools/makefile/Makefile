#hellowork
$(info Hello world)
# CURDIR is default variable
$(info $(CURDIR))

a=$(shell pwd)
# call no exist function
b=$(call dfdasddasfads123etect, dasf)


# ifeq ($(wildcard ./not_exist_file.config.mk),)
# $(error mk/config.mk: file not found. Please run configure before make)
# endif

ifneq ($(V),1)
Q ?= @
endif
S ?= $(notdir $(CURDIR))


all:
	$(Q)echo $(a)
	$(Q)echo $(b)
	$(Q)echo hello all
	echo $(S)


COMPILE_C=\
	$(Q)echo "  CC $S/$@"; \
	$(CC) -o $@ -c $<


%.o: %.c
	$(COMPILE_C)


clean:
	rm -f hello.o
