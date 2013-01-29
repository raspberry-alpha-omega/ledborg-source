# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
obj-m := ledborg.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:	driver

rebuild:	clean driver

clean:
	rm -rf .tmp_versions
	rm -f .*.cmd
	rm -f Module.symvers
	rm -f modules.order
	rm -f *.ko
	rm -f *.o
	rm -f *.mod.*
	rm -f ../ledborg.ko

driver:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	cp ledborg.ko ../ledborg.ko
endif

