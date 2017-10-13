obj-m := main_mod.o

KERNELDIR :=/lib/modules/`uname -r`/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.c .tmp_versions Module.symvers modules.order

