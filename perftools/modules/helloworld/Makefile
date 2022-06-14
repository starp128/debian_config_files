ifneq ($(KERNELRELEASE),)
obj-m := helloworld.o
else
KERNELDIR :=/lib/modules/`uname -r`/build
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *.ko *.mod.c .tmp_versions Module.symvers modules.order

