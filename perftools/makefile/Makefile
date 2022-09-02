#hellowork
$(info Hello world)
# CURDIR is default variable
$(info current direc is $(CURDIR))

a=$(shell pwd)
# call no exist function
b=$(call dfdasddasfads123etect, dasf)



########################################
########## variable ####################
########################################
$(info ########### begin variable ######)


# recursively expanded variables,
foo = $(bar)
bar = $(ugh)
ugh = Huh?

all:;echo $(foo)

# Simply expanded variables
# x := foo
# y := $(x) bar
# x := later
# is equivalent to

# y := foo bar
# x := later


$(info $(origin FOO))
FOO ?= bar
$(info $(FOO))
$(info $(origin FOO))

FOO += boo
$(info $(FOO))


# special vars
$(info default goal is $(.DEFAULT_GOAL))

$(info ########### end variable #######)
$(info)

########################################
##########condition#####################
########################################


ifeq (aaa, aaa)
$(info yes aa is aa)
endif

ifneq (aaa, bbb)
$(info aaa is not bbb)
endif

bar = true
foo = bar
ifdef $(foo)
frobozz = yes
$(info foo is defined)
endif

ifndef $(a_not_defined_var)
$(info not defined)
endif


$(info $(findstring t,$(MAKEFLAGS)))

ifneq ($(findstring t,$(var)),)
  $(info yyyyyyyyyyyyes)
else
  $(info noooooooooo)
endif

ifeq ($(wildcard ./not_exist_file.config.mk),)
#$(error mk/config.mk: file not found. Please run configure before make)
endif



########################################
########## function ####################
########################################
$(info ###### function ##)
$(info )

# $(function arguments)


# string function
$(info $(subst ee,EE,feet on the street))


$(info $(patsubst %.c,%.o,x.c.c bar.c))
#$(var:pattern=replacement)
objects = foo.o bar.o baz.o
$(info $(objects:.o=.c))
$(info $(patsubst %.o,%.c,$(objects)))


# file path function

$(info ###### end function ##)
$(info )

# 8.4 Functions for Conditionals

# foreach
dirs := a b c d
files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))

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
