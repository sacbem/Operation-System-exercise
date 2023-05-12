ifneq ($(KERNELRELEASE),)
# KBuild part of Makefile
obj-m += birthday.o

else
# Normal part of Makefile
#

# Kernel build directory specified by KDIR variable
# Default to running kernel's build directory if KDIR not set externally
KDIR ?= "/lib/modules/`uname -r`/build"

all:
	$(MAKE) -C "$(KDIR)" M=`pwd` modules

clean:
	$(MAKE) -C "$(KDIR)" M=`pwd` clean

endif